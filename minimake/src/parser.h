#ifndef PARSER_H
#define PARSER_H

enum line_type {
    LINE_RULE,
    LINE_VARIABLE,
    LINE_COMMAND,
    LINE_COMMENT,
    LINE_EMPTY,
    LINE_ERROR
};

enum line_type parse_line(const char *line);


#endif /* !PARSER_H */
