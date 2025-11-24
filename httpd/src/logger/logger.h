#ifndef LOGGER_H
#define LOGGER_H

#include <time.h>

#include "../config/config.h"
#include "../http/http.h"
#include "../utils/string/string.h"
int logger_init(struct config *config);

void log_request(struct string *server_name, const char *request_type,
                 const char *target, const char *client_ip);

void log_response(struct string *server_name, int status_code,
                  const char *client_ip, struct http_request *request);

void log_bad_request(const char *server_name, const char *client_ip);

void log_method_not_allowed(const char *server_name, int status_code,
                            const char *client_ip, const char *target);

void logger_cleanup(void);

#endif /* !LOGGER_H */
