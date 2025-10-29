#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"
#include "parser.h"

void add_variable(struct minimake_context *c, const char *name,
                  const char *value)
{
    c->var_count++;
    c->variables =
        realloc(c->variables, c->var_count * sizeof(struct variable *));

    struct variable *var = malloc(sizeof(struct variable));
    var->name = mystrdup(name);
    var->value = mystrdup(value);

    c->variables[c->var_count - 1] = var;
}

void add_rule(struct minimake_context *c, const char *target)
{
    c->rule_count++;
    c->rules = realloc(c->rules, c->rule_count * sizeof(struct rule *));

    struct rule *rule = malloc(sizeof(struct rule));
    rule->target = mystrdup(target);
    rule->dependencies = NULL;
    rule->dep_count = 0;
    rule->commands = NULL;
    rule->cmd_count = 0;

    c->rules[c->rule_count - 1] = rule;
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
        add_variable(c, expanded_name, value);
        free(expanded_name);
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
        add_rule(c, expanded_target);
        free(expanded_target);

        char *deps_copy = mystrdup(deps);
        char *dep = strtok(deps_copy, " \t");
        while (dep)
        {
            dep = trim_whitespace(dep);
            if (*dep != '\0')
            {
                char *expanded_dep = substitute_variables_recursive(dep, c);
                add_dependency(c, expanded_dep);
                free(expanded_dep);
            }
            dep = strtok(NULL, " \t");
        }
        free(deps_copy);
    }
    *in_recipe = 1;
    return 1;
}

int handle_command_line(char *line, struct minimake_context *c, int in_recipe)
{
    if (!in_recipe)
        return 0;
    char *cmd = line;
    while (*cmd && isspace(*cmd))
        cmd++;
    if (*cmd != '\0')
        add_command(c, cmd);
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
