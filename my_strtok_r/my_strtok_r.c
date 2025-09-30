#include "my_strtok_r.h"

static int is_delimiter(char c, const char *delim)
{
    for (const char *d = delim; *d != '\0'; d++)
    {
        if (c == *d)
            return 1;
    }
    return 0;
}

char *my_strtok_r(char *str, const char *delim, char **saveptr)
{
    if (str == NULL)
        str = *saveptr;

    if (str == NULL || *str == '\0')
    {
        *saveptr = NULL;
        return NULL;
    }

    while (*str != '\0' && is_delimiter(*str, delim))
        str++;

    if (*str == '\0')
    {
        *saveptr = NULL;
        return NULL;
    }

    char *token_start = str;

    while (*str != '\0' && !is_delimiter(*str, delim))
        str++;

    if (*str == '\0')
    {
        *saveptr = NULL;
    }
    else
    {
        *str = '\0';
        *saveptr = str + 1;
    }

    return token_start;
}
