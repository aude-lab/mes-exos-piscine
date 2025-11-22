#include "http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *my_strdup(const char *str)
{
    if (!str)
        return NULL;
    size_t len = strlen(str) + 1;
    char *copy = malloc(len);
    if (copy)
    {
        memcpy(copy, str, len);
    }
    return copy;
}

static char *trim_whitespace(char *str)
{
    if (!str)
        return NULL;
    char *start = str;
    while (*start && (*start == ' ' || *start == '\t'))
        start++;
    char *end = start + strlen(start) - 1;
    while (end > start
           && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'))
        end--;
    *(end + 1) = '\0';
    return start;
}

static int parse_request_line(struct http_request *request, const char *line)
{
    char *copy = my_strdup(line);
    if (!copy)
        return -1;

    char *token = strtok(copy, " ");
    if (token)
    {
        request->method = my_strdup(trim_whitespace(token));
        token = strtok(NULL, " ");
    }
    if (token)
    {
        request->target = my_strdup(trim_whitespace(token));
        token = strtok(NULL, " \r\n");
    }
    if (token)
    {
        request->version = my_strdup(trim_whitespace(token));
    }

    free(copy);
    return (request->method && request->target && request->version) ? 0 : -1;
}

static struct http_header *parse_header_line(const char *line)
{
    char *copy = my_strdup(line);
    if (!copy)
        return NULL;

    char *colon = strchr(copy, ':');
    if (!colon)
    {
        free(copy);
        return NULL;
    }

    *colon = '\0';
    struct http_header *header = malloc(sizeof(struct http_header));
    if (!header)
    {
        free(copy);
        return NULL;
    }

    header->name = my_strdup(trim_whitespace(copy));
    header->value = my_strdup(trim_whitespace(colon + 1));
    header->next = NULL;

    free(copy);

    if (!header->name || !header->value)
    {
        free(header->name);
        free(header->value);
        free(header);
        return NULL;
    }

    return header;
}

struct http_request *http_parse_request(struct string *raw_data)
{
    if (!raw_data || !raw_data->data)
        return NULL;

    struct http_request *request = calloc(1, sizeof(struct http_request));
    if (!request)
        return NULL;

    const char *data = raw_data->data;
    const char *line_start = data;
    int line_number = 0;
    size_t i;

    for (i = 0; i < raw_data->size; i++)
    {
        if (data[i] == '\n')
        {
            size_t line_len = &data[i] - line_start;
            if (line_len > 0)
            {
                char *line = malloc(line_len + 1);
                memcpy(line, line_start, line_len);
                line[line_len] = '\0';

                if (line_number == 0)
                {
                    if (parse_request_line(request, line) != 0)
                    {
                        free(line);
                        http_request_free(request);
                        return NULL;
                    }
                }
                else if (strlen(trim_whitespace(line)) > 0)
                {
                    struct http_header *header = parse_header_line(line);
                    if (header)
                    {
                        header->next = request->headers;
                        request->headers = header;
                    }
                }
                free(line);
            }

            line_start = &data[i + 1];
            line_number++;

            if (line_len == 0
                || (line_len == 1 && i > 0 && data[i - 1] == '\r'))
            {
                break;
            }
        }
    }

    return request;
}

const char *http_get_header(const struct http_request *request,
                            const char *name)
{
    if (!request || !name)
        return NULL;
    struct http_header *current = request->headers;
    while (current)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void http_request_free(struct http_request *request)
{
    if (!request)
        return;
    free(request->method);
    free(request->target);
    free(request->version);

    struct http_header *current = request->headers;
    while (current)
    {
        struct http_header *next = current->next;
        free(current->name);
        free(current->value);
        free(current);
        current = next;
    }

    if (request->body)
    {
        string_destroy(request->body);
    }
    free(request);
}
