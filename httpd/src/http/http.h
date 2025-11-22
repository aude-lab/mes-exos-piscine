#ifndef HTTP_H
#define HTTP_H

#include "../utils/string/string.h"

struct http_header
{
    char *name;
    char *value;
    struct http_header *next;
};

struct http_request
{
    char *method;
    char *target;
    char *version;
    struct http_header *headers;
    struct string *body;
};

struct http_request *http_parse_request(struct string *raw_data);
void http_request_free(struct http_request *request);
const char *http_get_header(const struct http_request *request,
                            const char *name);

#endif /* !HTTP_H */
