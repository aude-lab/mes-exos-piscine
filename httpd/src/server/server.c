#define _POSIX_C_SOURCE 200112L

#include "server.h"

#include <arpa/inet.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../daemon/daemon.h"
#include "../http/http.h"
#include "../http/response.h"
#include "../logger/logger.h"
#include "../utils/string/string.h"
#define BUFFER_SIZE 1024

static char *get_client_ip(int client_fd)
{
    static char ip_str[INET_ADDRSTRLEN];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if (getpeername(client_fd, (struct sockaddr *)&addr, &addr_len) == 0)
    {
        inet_ntop(AF_INET, &addr.sin_addr, ip_str, sizeof(ip_str));
        return ip_str;
    }

    return "unknown";
}

static void respond(int client_fd, const char *buffer, ssize_t bytes)
{
    ssize_t total = 0;
    ssize_t sent = 0;

    while (total != bytes)
    {
        sent = send(client_fd, buffer + total, bytes - total, 0);
        if (sent == -1)
        {
            if (errno == EINTR)
                continue;
            return;
        }
        total += sent;
    }
}

static int create_and_bind(const char *node, const char *service)
{
    struct addrinfo hints = { 0 };
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *res = NULL;

    if (getaddrinfo(node, service, &hints, &res) == -1)
    {
        return -1;
    }

    int sockfd = -1;
    struct addrinfo *p = NULL;

    for (p = res; p != NULL; p = p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1)
            continue;

        int optval = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval,
                       sizeof(optval))
            == -1)
        {
            close(sockfd);
            sockfd = -1;
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == 0)
            break;

        close(sockfd);
        sockfd = -1;
    }

    freeaddrinfo(res);

    if (p == NULL)
    {
        return -1;
    }

    return sockfd;
}

static int serve_file(const struct request_context *ctx, const char *filepath)
{
    int fd = open(filepath, O_RDONLY);
    if (fd == -1)
    {
        if (errno == EACCES)
            return -2;
        return -1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1)
    {
        close(fd);
        return -1;
    }

    char date[64];
    time_t now = time(NULL);
    struct tm *gmt_time = gmtime(&now);
    strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S GMT", gmt_time);

    char headers[512];
    int headers_len = snprintf(headers, sizeof(headers),
                               "HTTP/1.1 200 OK\r\n"
                               "Date: %s\r\n"
                               "Content-Length: %ld\r\n"
                               "Connection: close\r\n"
                               "Content-Type: %s\r\n"
                               "\r\n",
                               date, st.st_size, get_content_type(filepath));

    if (headers_len > 0)
        respond(ctx->client_fd, headers, headers_len);

    if (strcmp(ctx->request->method, "HEAD") == 0)
    {
        close(fd);
        log_response(ctx->config->servers->server_name, 200, ctx->client_ip,
                     ctx->request);
        return 0;
    }

    off_t offset = 0;
    ssize_t sent = sendfile(ctx->client_fd, fd, &offset, st.st_size);
    close(fd);

    if (sent == -1)
        return -1;

    log_response(ctx->config->servers->server_name, 200, ctx->client_ip,
                 ctx->request);
    return 0;
}

static void send_error_response(const struct request_context *ctx,
                                int status_code)
{
    struct string *response = NULL;

    switch (status_code)
    {
    case 400:
        response = http_create_400_response();
        break;
    case 405:
        response = http_create_405_response();
        break;
    case 403:
        response = http_create_403_response();
        break;
    case 404:
        response = http_create_404_response();
        break;
    case 500:
        response = http_create_500_response();
        break;
    case 505:
        response = http_create_505_response();
        break;
    default:
        return;
    }

    if (response)
    {
        respond(ctx->client_fd, response->data, response->size);
        string_destroy(response);
    }

    log_response(ctx->config->servers->server_name, status_code, ctx->client_ip,
                 ctx->request);
}

static struct string *read_client_request(int client_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

    if (bytes <= 0)
        return NULL;

    buffer[bytes] = '\0';
    return string_create(buffer, bytes);
}

static int validate_and_log_request(struct http_request *request,
                                    struct config *config,
                                    const char *client_ip)
{
    if (!request || !request->method || !request->target)
        return 400;

    if (strcmp(request->version, "HTTP/1.1") != 0)
        return 505;

    if (strcmp(request->method, "GET") != 0
        && strcmp(request->method, "HEAD") != 0)
        return 405;

    log_request(config->servers->server_name, request->method, request->target,
                client_ip);
    return 0;
}

static void handle_file_request(const struct request_context *ctx)
{
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "%s%s", ctx->config->servers->root_dir,
             ctx->request->target);

    struct stat path_stat;
    if (stat(filepath, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
    {
        size_t len = strlen(filepath);
        if (len > 0 && filepath[len - 1] != '/')
            strncat(filepath, "/", sizeof(filepath) - len - 1);
        strncat(filepath, ctx->config->servers->default_file,
                sizeof(filepath) - strlen(filepath) - 1);
    }

    int result = serve_file(ctx, filepath);
    if (result == -2)
        send_error_response(ctx, 403);
    else if (result != 0)
        send_error_response(ctx, 404);
}

static void communicate(int client_fd, struct config *config)
{
    char *client_ip = get_client_ip(client_fd);
    struct string *raw_request = read_client_request(client_fd);

    if (!raw_request)
    {
        close(client_fd);
        return;
    }

    struct http_request *request = http_parse_request(raw_request);
    struct request_context ctx = { client_fd, config, client_ip, request };

    int error_code = validate_and_log_request(request, config, client_ip);

    if (error_code == 400)
        log_bad_request(config->servers->server_name->data, client_ip);

    if (error_code != 0)
    {
        if (error_code == 405)
        {
            log_method_not_allowed(config->servers->server_name->data,
                                   error_code, client_ip, request->target);
            struct string *response = http_create_405_response();
            if (response)
            {
                respond(client_fd, response->data, response->size);
                string_destroy(response);
            }
        }
        else
            send_error_response(&ctx, error_code);
    }
    else
    {
        handle_file_request(&ctx);
    }

    if (request)
        http_request_free(request);
    string_destroy(raw_request);
    close(client_fd);
}

static void start_server(int server_socket, struct config *config)
{
    if (listen(server_socket, SOMAXCONN) == -1)
    {
        fprintf(stderr, "listen failed: %s\n", strerror(errno));
        return;
    }

    while (!is_shutdown_requested())
    {
        int cfd = accept(server_socket, NULL, NULL);
        if (cfd != -1)
        {
            communicate(cfd, config);
            close(cfd);
        }
        else
        {
            if (errno == EINTR)
                continue;
        }
    }
}

int start_echo_server(const char *ip, const char *port, struct config *config)
{
    int socket_fd = create_and_bind(ip, port);
    if (socket_fd == -1)
    {
        return -1;
    }

    start_server(socket_fd, config);

    close(socket_fd);
    return 0;
}

int start_http_server(const char *ip, const char *port, struct config *config)
{
    return start_echo_server(ip, port, config);
}
