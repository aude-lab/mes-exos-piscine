#ifndef LOGGER_H
#define LOGGER_H

#include <time.h>
#include "../config/config.h"

int logger_init(struct config *config);

void log_request(const char *server_name, const char *request_type, 
                 const char *target, const char *client_ip);

void log_response(const char *server_name, int status_code, 
                  const char *client_ip, const char *request_type, 
                  const char *target);

void log_bad_request(const char *server_name, const char *client_ip);

void log_method_not_allowed(const char *server_name, int status_code,
                            const char *client_ip, const char *target);

void logger_cleanup(void);

#endif /* !LOGGER_H */
