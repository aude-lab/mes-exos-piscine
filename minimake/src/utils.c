#include <stdlib.h>
#include <string.h>

#include "main.h"

char *mystrdup(const char *s)
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

void free_variable(struct variable *var)
{
    if (var == NULL)
        return;
    free(var->name);
    free(var->value);
    free(var);
}

void free_rule(struct rule *rule)
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

void free_all_variables(struct minimake_context *c)
{
    for (int i = 0; i < c->var_count; i++)
        free_variable(c->variables[i]);
    free(c->variables);
    c->variables = NULL;
    c->var_count = 0;
}

void free_all_rules(struct minimake_context *c)
{
    for (int i = 0; i < c->rule_count; i++)
        free_rule(c->rules[i]);
    free(c->rules);
    c->rules = NULL;
    c->rule_count = 0;
}

void free_targets(struct minimake_context *c)
{
    for (int i = 0; i < c->target_count; i++)
        free(c->targets[i]);
    free(c->targets);
    c->targets = NULL;
    c->target_count = 0;
}

void free_minimake_context(struct minimake_context *c)
{
    free_targets(c);
    free_all_variables(c);
    free_all_rules(c);
}
