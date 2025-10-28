#ifndef MAIN_H
#define MAIN_H

#include <time.h>

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

// Fonctions principales
void init_minimake(struct minimake_context *c);
void free_minimake_context(struct minimake_context *c);
int parse_arguments(int argc, char *argv[], struct minimake_context *c);
int read_makefile(const char *filename, struct minimake_context *c);
void help_option(void);

// Construction
int build_target(struct minimake_context *ctx, const char *target_name);
struct rule *find_default_target(struct minimake_context *ctx);

// Pretty print
void pretty_print(struct minimake_context *c);

// Command execution
int should_log_command(const char *command);
int execute_command(const char *command, int should_log);

// File utilities
int file_exists(const char *filename);
time_t get_file_mtime(const char *filename);

// Variable substitution
char *find_variable_value(struct minimake_context *c, const char *name);
char *substitute_variables(const char *input, struct minimake_context *c);
char *substitute_variables_recursive(const char *input,
                                     struct minimake_context *ctx);
char *mystrdup(const char *s);

// Variable handlers
int handle_brace_variable(const char *ptr, struct minimake_context *c,
                          char **out_ptr, const char **next_ptr);
int handle_single_variable(const char *ptr, struct minimake_context *ctx,
                           char **out_ptr, const char **next_ptr);

#endif /* !MAIN_H */
