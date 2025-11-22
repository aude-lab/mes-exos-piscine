#include "string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct string *string_create(const char *str, size_t size)
{
    struct string *new = malloc(sizeof(struct string));
    new->size = size;
    if (size <= 0)
    {
        new->data = NULL;
        return new;
    }
    new->data = malloc(size);
    if (!new->data)
    {
        free(new);
        return NULL;
    }
    memcpy(new->data, str, size);

    return new;
}

int string_compare_n_str(const struct string *str1, const char *str2, size_t n)
{
    if (!str1 || !str2)
        return 0;
    if (n == 0)
        return 0;
    size_t compare_size = (str1->size < n) ? str1->size : n;
    return memcmp(str1->data, str2, compare_size);
}

void string_concat_str(struct string *str, const char *to_concat, size_t size)
{
    if (!str || size == 0)
        return;
    if (!to_concat)
        return;
    size_t new_size = str->size + size;
    char *new_data = realloc(str->data, new_size);
    if (!new_data)
        return;
    str->data = new_data;
    memcpy(str->data + str->size, to_concat, size);
    str->size = new_size;
}

void string_destroy(struct string *str)
{
    if (!str)
        return;
    if (str->data)
        free(str->data);
    free(str);
}
