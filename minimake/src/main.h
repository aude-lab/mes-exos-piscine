#ifndef MAIN_H
#define MAIN_H

struct minimake_context
{
    char *makefile_name;
    int pretty_print;
    int show_help;
    char **targets;
    int target_count;
    struct variable **variables;
    int var_count;
    struct rule **rules;
    int rule_count;
};

struct variable
{
    char *name;
    char *value;
};

struct rule
{
    char *target;
    char **dependencies;
    char **commands;
    int dep_count;
    int cmd_count;
};

#endif /* !MAIN_H */
