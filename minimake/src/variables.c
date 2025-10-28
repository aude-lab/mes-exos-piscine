#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

char *find_variable_value(struct minimake_context *c, const char *name)
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

int handle_brace_variable(const char *ptr, struct minimake_context *c, char **out_ptr, const char **next_ptr)
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
        *out_ptr += value_len;
    }

    free(var_name);    
    *next_ptr = var_end + 1;
    return 1;
}

int handle_single_variable(const char *ptr, struct minimake_context *ctx, char **out_ptr, const char **next_ptr)
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

char *substitute_variables_recursive(const char *input,
                                     struct minimake_context *ctx)
{
    if (input == NULL)
        return NULL;

    char *current = mystrdup(input);
    char *next;
    int changed;
    int max_iterations = 10;
    int iteration = 0;

    while (changed && iteration < max_iterations)
    {
        next = substitute_variables(current, ctx);
        changed = (next && strcmp(current, next) != 0);

        if (changed)
        {
            free(current);
            current = next;
        }
        else
        {
            free(next);
        }

        iteration++;
    }
    return current;
}
