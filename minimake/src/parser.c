#include "parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum line_type parse_line(const char *line)
{
    while (isspace(*line) && *line != '\t')
    {
        line++;
    }

    if (*line == '\0')
    {
        return LINE_EMPTY;
    }

    if (*line == '#')
    {
        return LINE_COMMENT;
    }

    if (*line == '\t')
    {
        return LINE_COMMAND;
    }

    const char *ptr = line;
    while (*ptr && *ptr != ':' && *ptr != '=' && *ptr != '#')
    {
        ptr++;
    }

    if (*ptr == ':')
    {
        return LINE_RULE;
    }
    else if (*ptr == '=')
    {
        return LINE_VARIABLE;
    }

    return LINE_ERROR;
}
