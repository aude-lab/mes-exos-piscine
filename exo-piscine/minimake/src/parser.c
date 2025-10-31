#include "parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum line_type parse_line(const char *line)
{
    printf("DEBUG: parsing line: '%s'\n", line); // À retirer après
    const char *ptr = line;

    while (isspace(*ptr) && *ptr != '\t')
    {
        ptr++;
    }

    if (*ptr == '\0')
    {
        return LINE_EMPTY;
    }

    if (*ptr == '#')
    {
        return LINE_COMMENT;
    }

    if (*ptr == '\t')
    {
        return LINE_COMMAND;
    }

    ptr = line;
    while (isspace(*ptr))
    {
        ptr++;
    }

    if (*ptr == '\0')
        return LINE_EMPTY;
    if (*ptr == '#')
        return LINE_COMMENT;

    const char *ptr2 = ptr;
    while (*ptr2 && *ptr2 != ':' && *ptr2 != '=' && *ptr2 != '#')
    {
        ptr2++;
    }

    if (*ptr2 == ':')
    {
        return LINE_RULE;
    }
    else if (*ptr2 == '=')
    {
        return LINE_VARIABLE;
    }

    return LINE_ERROR;
}
