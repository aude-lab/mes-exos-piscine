#ifndef RESPONSE_H
#define RESPONSE_H

#include "../utils/string/string.h"

struct string *http_create_response(int status_code, const char *status_text,
                                    const char *content_type, const char *body);
struct string *http_create_400_response(void);
struct string *http_create_404_response(void);
struct string *http_create_500_response(void);
struct string *http_create_405_response(void);
struct string *http_create_505_response(void);
struct string *http_create_403_response(void);
const char *get_content_type(const char *filename);

#endif /* !RESPONSE_H */
