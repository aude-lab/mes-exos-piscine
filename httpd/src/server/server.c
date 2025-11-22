#define _POSIX_C_SOURCE 200112L

#include "server.h"
#include <err.h>
#include <errno.h>
#include <netdb.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "../logger/logger.h"
#include "../http/http.h"
#include "../utils/string/string.h"

#define BUFFER_SIZE 1024
#define LOGGER(...) fprintf(stderr, __VA_ARGS__)

static char *get_client_ip(int client_fd)
{
    static char ip_str[INET_ADDRSTRLEN];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    
    if (getpeername(client_fd, (struct sockaddr*)&addr, &addr_len) == 0)
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
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
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

static void communicate(int client_fd, struct config *config)
{
    ssize_t bytes = 0;
    char buffer[BUFFER_SIZE];

    char *client_ip = get_client_ip(client_fd);

    bytes = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes > 0)
    {
        buffer[bytes] = '\0';

        struct string *raw_request = string_create(buffer, bytes);
        if (!raw_request) {
            log_bad_request(config->servers->server_name->data, client_ip);
            close(client_fd);
            return;
        }

        struct http_request *request = http_parse_request(raw_request);

        if (request && request->method && request->target) {
            log_request(config->servers->server_name->data,
                       request->method, request->target, client_ip);

            printf("Method: %s, Target: %s, Version: %s\n",
                   request->method, request->target, request->version);

            struct http_header *header = request->headers;
            while (header) {
                printf("   Header: %s: %s\n", header->name, header->value);
                header = header->next;
            }

            log_response(config->servers->server_name->data, 200,
                        client_ip, request->method, request->target);
        } else {
            log_bad_request(config->servers->server_name->data, client_ip);
        }

        if (request) http_request_free(request);
        string_destroy(raw_request);
        respond(client_fd, buffer, bytes);
    }
    else if (bytes == -1)
    {
        fprintf(stderr, "recv failed: %s\n", strerror(errno));
    }

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
    LOGGER("Starting HTTP server (echo mode for now)\n");
    return start_echo_server(ip, port, config);
}
