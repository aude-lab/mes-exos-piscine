#include "response.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *get_content_type(const char *filename)
{
    const char *ext = strrchr(filename, '.');
    if (!ext)
        return "text/plain";

    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0)
        return "text/html";
    else if (strcmp(ext, ".css") == 0)
        return "text/css";
    else if (strcmp(ext, ".js") == 0)
        return "application/javascript";
    else if (strcmp(ext, ".png") == 0)
        return "image/png";
    else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
        return "image/jpeg";
    else if (strcmp(ext, ".txt") == 0)
        return "text/plain";
    else
        return "application/octet-stream";
}

struct string *http_create_response(int status_code, const char *status_text,
                                    const char *content_type, const char *body)
{
    char date[64];
    time_t now = time(NULL);
    struct tm *gmt_time = gmtime(&now);
    strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S GMT", gmt_time);
    size_t body_len = body ? strlen(body) : 0;

    char response[4096];
    int len = snprintf(response, sizeof(response),
                       "HTTP/1.1 %d %s\r\n"
                       "Date: %s\r\n"
                       "Content-Length: %zu\r\n"
                       "Content-Type: %s\r\n"
                       "Connection: close\r\n"
                       "\r\n"
                       "%s",
                       status_code, status_text, date, body_len, content_type,
                       body ? body : "");

    size_t new_len = len;
    if (len < 0 || new_len >= sizeof(response))
    {
        return NULL;
    }

    return string_create(response, len);
}

struct string *http_create_400_response(void)
{
    const char *body = "<html>\n"
                       "<head><title>400 Bad Request</title></head>\n"
                       "<body>\n"
                       "<h1>400 Bad Request</h1>\n"
                       "<p>Your browser sent a request that this server could "
                       "not understand.</p>\n"
                       "</body>\n"
                       "</html>\n";

    return http_create_response(400, "Bad Request", "text/html", body);
}

struct string *http_create_404_response(void)
{
    const char *body =
        "<html>\n"
        "<head><title>404 Not Found</title></head>\n"
        "<body>\n"
        "<h1>404 Not Found</h1>\n"
        "<p>The requested resource was not found on this server.</p>\n"
        "</body>\n"
        "</html>\n";

    return http_create_response(404, "Not Found", "text/html", body);
}

struct string *http_create_500_response(void)
{
    const char *body = "<html>\n"
                       "<head><title>500 Internal Server Error</title></head>\n"
                       "<body>\n"
                       "<h1>500 Internal Server Error</h1>\n"
                       "<p>Something went wrong on the server.</p>\n"
                       "</body>\n"
                       "</html>\n";

    return http_create_response(500, "Internal Server Error", "text/html",
                                body);
}

struct string *http_create_403_response(void)
{
    const char *body =
        "<html>\n"
        "<head><title>403 Forbidden</title></head>\n"
        "<body>\n"
        "<h1>403 Forbidden</h1>\n"
        "<p>You don't have permission to access this resource.</p>\n"
        "</body>\n"
        "</html>\n";

    return http_create_response(403, "Forbidden", "text/html", body);
}

struct string *http_create_405_response(void)
{
    const char *body = "<html>\n"
                       "<head><title>405 Method Not Allowed</title></head>\n"
                       "<body>\n"
                       "<h1>405 Method Not Allowed</h1>\n"
                       "<p>The method is not allowed for this resource.</p>\n"
                       "</body>\n"
                       "</html>\n";

    return http_create_response(405, "Method Not Allowed", "text/html", body);
}

struct string *http_create_505_response(void)
{
    const char *body =
        "<html>\n"
        "<head><title>505 HTTP Version Not Supported</title></head>\n"
        "<body>\n"
        "<h1>505 HTTP Version Not Supported</h1>\n"
        "<p>The HTTP version is not supported by this server.</p>\n"
        "</body>\n"
        "</html>\n";

    return http_create_response(505, "HTTP Version Not Supported", "text/html",
                                body);
}
