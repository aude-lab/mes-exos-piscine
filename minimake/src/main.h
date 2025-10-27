#ifndef MAIN_H
#define MAIN_H

struct minimake_context
{
    char *makefile_name;
    int pretty_print;
    int show_help;
    char **targets;
    int target_count;
};

#endif /* !MAIN_H */
