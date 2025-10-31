#include "parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum line_type parse_line(const char *line)
{
    const char *ptr = line;

    if (*ptr == '\t')
        return LINE_COMMAND;

    while (*ptr && isspace(*ptr))
    {
        ptr++;
    }

    if (*ptr == '\0')
        return LINE_EMPTY;

    if (*ptr == '#')
        return LINE_COMMENT;

    const char *search = ptr;
    while (*search && *search != ':' && *search != '=' && *search != '#')
    {
        search++;
    }

    if (*search == ':')
        return LINE_RULE;
    else if (*search == '=')
        return LINE_VARIABLE;

    if (line[0] == '\t')
        return LINE_COMMAND;

    return LINE_ERROR;
}
