#ifndef SERVER_H
#define SERVER_H

#include "../config/config.h"

struct request_context
{
    int client_fd;
    struct config *config;
    const char *client_ip;
    struct http_request *request;
};

int start_echo_server(const char *ip, const char *port, struct config *config);
int start_http_server(const char *ip, const char *port, struct config *config);

#endif /* SERVER_H */
