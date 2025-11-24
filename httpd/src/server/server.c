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

#include "../http/http.h"
#include "../http/response.h"
#include "../logger/logger.h"
#include "../utils/string/string.h"

#define BUFFER_SIZE 1024
#define LOGGER(...) fprintf(stderr, __VA_ARGS__)

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
            LOGGER("Send failed: %s\n", strerror(errno));
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
        LOGGER("create_and_bind: failed getaddrinfo\n");
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
        LOGGER("create_and_bind: failed to bind\n");
        return -1;
    }

    return sockfd;
}

static int serve_file(const struct request_context *ctx, const char *filepath)
{
    int fd = open(filepath, O_RDONLY);
    if (fd == -1)
        return -1;

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
    case 404:
        response = http_create_404_response();
        break;
    case 500:
        response = http_create_500_response();
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

static void communicate(int client_fd, struct config *config)
{
    ssize_t bytes = 0;
    char buffer[BUFFER_SIZE];
    char *client_ip = get_client_ip(client_fd);

    bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

    if (bytes <= 0)
    {
        close(client_fd);
        return;
    }

    buffer[bytes] = '\0';

    struct string *raw_request = string_create(buffer, bytes);
    if (!raw_request)
    {
        log_bad_request(config->servers->server_name->data, client_ip);
        struct request_context ctx = { client_fd, config, client_ip, NULL };
        send_error_response(&ctx, 400);
        close(client_fd);
        return;
    }

    struct http_request *request = http_parse_request(raw_request);
    struct request_context ctx = { client_fd, config, client_ip, request };

    if (request && request->method && request->target)
    {
        log_request(config->servers->server_name, request->method,
                    request->target, client_ip);

        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s%s", config->servers->root_dir,
                 request->target);

        if (serve_file(&ctx, filepath) != 0)
            send_error_response(&ctx, 404);
    }
    else
    {
        log_bad_request(config->servers->server_name->data, client_ip);
        send_error_response(&ctx, 400);
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
        LOGGER("listen failed: %s\n", strerror(errno));
        return;
    }

    LOGGER("OKAYY Server listening...\n");

    while (1)
    {
        int cfd = accept(server_socket, NULL, NULL);
        if (cfd != -1)
        {
            LOGGER("Client connected\n");
            communicate(cfd, config);
            close(cfd);
            LOGGER("Client disconnected\n");
        }
        else
        {
            LOGGER("accept failed: %s\n", strerror(errno));
        }
    }
}

int start_echo_server(const char *ip, const char *port, struct config *config)
{
    LOGGER("Starting echo server on %s:%s\n", ip, port);

    int socket_fd = create_and_bind(ip, port);
    if (socket_fd == -1)
    {
        LOGGER("Failed to create and bind socket\n");
        return -1;
    }

    start_server(socket_fd, config);

    close(socket_fd);
    return 0;
}

int start_http_server(const char *ip, const char *port, struct config *config)
{
    LOGGER("Starting HTTP server\n");
    return start_echo_server(ip, port, config);
}
