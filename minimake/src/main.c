#define _POSIX_C_SOURCE 200809L
#include "main.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static char *mystrdup(const char *s)
{
    if (s == NULL)
        return NULL;

    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (copy != NULL)
    {
        memcpy(copy, s, len);
    }
    return copy;
}

void init_minimake(struct minimake_context *c)
{
    c->makefile_name = NULL;
    c->pretty_print = 0;
    c->show_help = 0;
    c->targets = NULL;
    c->target_count = 0;
    c->variables = NULL;
    c->var_count = 0;
    c->rules = NULL;
    c->rule_count = 0;
}

void help_option(void)
{
    printf("Use: minimake [options] [targets]\n");
    printf("Options:\n");
    printf("  -f file    Using file as makefile\n");
    printf("  -h         Showing this help message\n");
    printf("  -p         Pretty-printing the makefile\n");
}

static void free_variable(struct variable *var)
{
    if (var == NULL)
        return;
    free(var->name);
    free(var->value);
    free(var);
}

static void free_rule(struct rule *rule)
{
    if (rule == NULL)
        return;

    free(rule->target);

    for (int i = 0; i < rule->dep_count; i++)
        free(rule->dependencies[i]);
    free(rule->dependencies);

    for (int i = 0; i < rule->cmd_count; i++)
        free(rule->commands[i]);
    free(rule->commands);

    free(rule);
}

static void free_all_variables(struct minimake_context *c)
{
    for (int i = 0; i < c->var_count; i++)
        free_variable(c->variables[i]);
    free(c->variables);
    c->variables = NULL;
    c->var_count = 0;
}

static void free_all_rules(struct minimake_context *c)
{
    for (int i = 0; i < c->rule_count; i++)
        free_rule(c->rules[i]);
    free(c->rules);
    c->rules = NULL;
    c->rule_count = 0;
}

static void free_targets(struct minimake_context *c)
{
    for (int i = 0; i < c->target_count; i++)
        free(c->targets[i]);
    free(c->targets);
    c->targets = NULL;
    c->target_count = 0;
}

static void free_minimake_context(struct minimake_context *c)
{
    free_targets(c);
    free_all_variables(c);
    free_all_rules(c);
}

int parse_arguments(int argc, char *argv[], struct minimake_context *c)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '\0')
        {
            fprintf(stderr,
                    "minimake: *** empty string invalid as argument. Stop.\n");
            return 2;
        }
        if (argv[i][0] == '-')
        {
            if (strcmp(argv[i], "-f") == 0)
            {
                if (i + 1 >= argc)
                {
                    fprintf(stderr, "minimake: *** missing argument to -f\n");
                    return 2;
                }
                c->makefile_name = argv[++i];
            }
            else if (strcmp(argv[i], "-h") == 0)
                c->show_help = 1;
            else if (strcmp(argv[i], "-p") == 0)
                c->pretty_print = 1;
            else
            {
                fprintf(stderr, "minimake: *** unknown option: %s\n", argv[i]);
                return 2;
            }
        }
        else
        {
            c->target_count++;
            c->targets = realloc(c->targets, c->target_count * sizeof(char *));
            c->targets[c->target_count - 1] = mystrdup(argv[i]);
        }
    }

    return 0;
}

static void add_variable(struct minimake_context *ctx, const char *name,
                         const char *value)
{
    ctx->var_count++;
    ctx->variables =
        realloc(ctx->variables, ctx->var_count * sizeof(struct variable *));

    struct variable *var = malloc(sizeof(struct variable));
    var->name = mystrdup(name);
    var->value = mystrdup(value);

    ctx->variables[ctx->var_count - 1] = var;
}

static void add_rule(struct minimake_context *ctx, const char *target)
{
    ctx->rule_count++;
    ctx->rules = realloc(ctx->rules, ctx->rule_count * sizeof(struct rule *));

    struct rule *rule = malloc(sizeof(struct rule));
    rule->target = mystrdup(target);
    rule->dependencies = NULL;
    rule->dep_count = 0;
    rule->commands = NULL;
    rule->cmd_count = 0;

    ctx->rules[ctx->rule_count - 1] = rule;
}

static void add_dependency(struct minimake_context *c, const char *dep)
{
    if (c->rule_count == 0)
        return;

    struct rule *rule = c->rules[c->rule_count - 1];
    rule->dep_count++;
    rule->dependencies =
        realloc(rule->dependencies, rule->dep_count * sizeof(char *));
    rule->dependencies[rule->dep_count - 1] = mystrdup(dep);
}

static char *find_variable_value(struct minimake_context *c, const char *name)
{
    for (int i = 0; i < c->var_count; i++)
    {
        if (strcmp(c->variables[i]->name, name) == 0)
        {
            return c->variables[i]->value;
        }
    }
    return NULL;
}

int handle_brace_variable(const char *ptr, struct minimake_context *c,
                          char **out_ptr, const char **next_ptr)
{
    char delim_start = *(ptr + 1);

    char delim_end;

    if (delim_start == '(')
        delim_end = ')';
    else
        delim_end = '}';

    const char *var_start = ptr + 2;

    const char *var_end = strchr(var_start, delim_end);

    if (var_end == NULL)
    {
        **out_ptr = '$';
        (*out_ptr)++;
        *next_ptr = ptr + 1;
        return 1;
    }

    size_t var_len = var_end - var_start;
    char *var_name = malloc(var_len + 1);
    strncpy(var_name, var_start, var_len);
    var_name[var_len] = '\0';

    char *value = find_variable_value(c, var_name);
    if (value)
    {
        size_t value_len = strlen(value);
        memcpy(*out_ptr, value, value_len);
        *out_ptr += strlen(value);
    }

    free(var_name);
    *next_ptr = var_end + 1;
    return 1;
}

int handle_single_variable(const char *ptr, struct minimake_context *ctx,
                           char **out_ptr, const char **next_ptr)
{
    if (!isalnum(*(ptr + 1)))
        return 0;

    char var_name[2] = { *(ptr + 1), '\0' };
    char *value = find_variable_value(ctx, var_name);
    if (value)
    {
        strcpy(*out_ptr, value);
        *out_ptr += strlen(value);
    }

    *next_ptr = ptr + 2;
    return 1;
}

char *substitute_variables(const char *input, struct minimake_context *c)
{
    if (input == NULL)
        return NULL;

    char *result = malloc(strlen(input) * 3 + 1);
    if (result == NULL)
        return NULL;
    result[0] = '\0';

    const char *ptr = input;
    char *out = result;

    while (*ptr)
    {
        if (*ptr == '$')
        {
            if (*(ptr + 1) == '$')
            {
                *out++ = '$';
                ptr += 2;
                continue;
            }

            if (*(ptr + 1) == '(' || *(ptr + 1) == '{')
            {
                if (handle_brace_variable(ptr, c, &out, &ptr))
                {
                    continue;
                }
            }

            if (handle_single_variable(ptr, c, &out, &ptr))
            {
                continue;
            }
        }

        *out++ = *ptr++;
    }

    *out = '\0';
    return result;
}


char *substitute_variables_recursive(const char *input, struct minimake_context *ctx)
{
    if (input == NULL) return NULL;

    char *current = mystrdup(input);
    char *next;
    int changed;
    int max_iterations = 10;
    int iteration = 0;

    do {
        next = substitute_variables(current, ctx);
        changed = (next && strcmp(current, next) != 0);

        if (changed) {
            free(current);
            current = next;
        } else {
            free(next); 
        }

        iteration++;
    } while (changed && iteration < max_iterations);

    return current;
    
}


static void add_command(struct minimake_context *c, const char *cmd)
{
    if (c->rule_count == 0)
        return;

    struct rule *rule = c->rules[c->rule_count - 1];
    rule->cmd_count++;
    rule->commands = realloc(rule->commands, rule->cmd_count * sizeof(char *));
    rule->commands[rule->cmd_count - 1] = mystrdup(cmd);

    char *expanded_cmd = substitute_variables(cmd, c);
    rule->commands[rule->cmd_count - 1] = mystrdup(expanded_cmd);
    free(expanded_cmd);
}

static char *trim_whitespace(char *str)
{
    if (str == NULL)
        return NULL;

    while (*str && isspace(*str))
        str++;

    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
        end--;
    end[1] = '\0';

    return str;
}

int handle_variable_line(char *line, struct minimake_context *c)
{
    char *equal = strchr(line, '=');
    if (equal == NULL)
        return 0;
    *equal = '\0';
    char *name = trim_whitespace(line);
    char *value = trim_whitespace(equal + 1);
    if (*name != '\0')
    {
	char *expanded_name = substitute_variables_recursive(name, c);
	char *expanded_value = substitute_variables_recursive(value, c);
        add_variable(c, name, value);
	free(expanded_name);
        free(expanded_value);
    }
    return 1;
}

int handle_rule_line(char *line, struct minimake_context *c, int *in_recipe)
{
    char *colon = strchr(line, ':');
    if (colon == NULL)
        return 0;
    *colon = '\0';
    char *target = trim_whitespace(line);
    char *deps = colon + 1;
    if (*target != '\0')
    {
	char *expanded_target = substitute_variables_recursive(target, c);
        add_rule(c, target);
	free(expanded_target);

        char *dep = strtok(deps, " \t");
        while (dep)
        {
            dep = trim_whitespace(dep);
            if (*dep != '\0')
            {
		char *expanded_dep = substitute_variables_recursive(dep, c);
                add_dependency(c, dep);
		free(expanded_dep);
	    }
            dep = strtok(NULL, " \t");
        }
    }
    *in_recipe = 1;
    return 1;
}

int handle_command_line(char *line, struct minimake_context *ctx, int in_recipe)
{
    if (!in_recipe)
        return 0;
    char *cmd = line;
    while (*cmd && isspace(*cmd))
        cmd++;
    if (*cmd != '\0')
        add_command(ctx, cmd);
    return 1;
}

int read_makefile(const char *filename, struct minimake_context *c)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "minimake: *** %s: %s\n", filename, strerror(errno));
        return 2;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int in_recipe = 0;

    while ((read = getline(&line, &len, file)) != -1)
    {
        if (read > 0 && line[read - 1] == '\n')
            line[read - 1] = '\0';

        enum line_type type = parse_line(line);

        switch (type)
        {
        case LINE_VARIABLE:
            handle_variable_line(line, c);
            in_recipe = 0;
            break;
        case LINE_RULE:
            handle_rule_line(line, c, &in_recipe);
            break;
        case LINE_COMMAND:
            handle_command_line(line, c, in_recipe);
            break;
        case LINE_EMPTY:
        case LINE_COMMENT:
            in_recipe = 0;
            break;
        case LINE_ERROR:
            fprintf(stderr, "minimake: *** syntax error\n");
            free(line);
            fclose(file);
            return 2;
        }
    }

    free(line);
    fclose(file);
    return 0;
}

void pretty_print(struct minimake_context *c)
{
    if (c->var_count > 0)
    {
        printf("# variables\n");
        for (int i = 0; i < c->var_count; i++)
            printf("'%s' = '%s'\n", c->variables[i]->name,
                   c->variables[i]->value);
    }
    if (c->rule_count > 0)
    {
        printf("# rules\n");
        for (int i = 0; i < c->rule_count; i++)
        {
            printf("(%s):", c->rules[i]->target);
            for (int i = 0; i < c->rules[i]->dep_count; i++)
                printf(" [%s]", c->rules[i]->dependencies[i]);
            printf("\n");

            for (int k = 0; k < c->rules[i]->cmd_count; k++)
                printf("    '%s'\n", c->rules[i]->commands[k]);
        }
    }
}

int main(int argc, char *argv[])
{
    struct minimake_context c;

    init_minimake(&c);

    if (parse_arguments(argc, argv, &c) != 0)
    {
        free_minimake_context(&c);
        return 2;
    }
    if (c.show_help == 1)
    {
        help_option();
        free_minimake_context(&c);
        return 0;
    }
    char *makefile_to_read = c.makefile_name;

    if (makefile_to_read == NULL)
    {
        FILE *test = fopen("makefile", "r");
        if (test)
        {
            makefile_to_read = "makefile";
            fclose(test);
        }
        else
        {
            test = fopen("Makefile", "r");
            if (test)
            {
                makefile_to_read = "Makefile";
                fclose(test);
            }
        }
    }

    if (makefile_to_read == NULL)
    {
        fprintf(stderr,
                "minimake: *** No targets specified and no makefile found.\n");
        free_minimake_context(&c);
        return 2;
    }

    if (read_makefile(makefile_to_read, &c) != 0)
    {
        free_minimake_context(&c);
        return 2;
    }

    if (c.pretty_print)
    {
        pretty_print(&c);
        free_minimake_context(&c);
        return 0;
    }

    free_minimake_context(&c);
    return 0;
}
