#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "main.h"
#include "parser.h"

struct rule *find_rule(struct minimake_context *c, const char *target_name)
{
    for (int i = 0; i < c->rule_count; i++)
    {
        if (strcmp(c->rules[i]->target, target_name) == 0)
        {
            return c->rules[i];
        }
    }
    return NULL;
}

int file_exists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return 1;
    }
    return 0;
}

time_t get_file_mtime(const char *filename)
{
    struct stat file_stat;
    if (stat(filename, &file_stat) == 0)
    {
        return file_stat.st_mtime;
    }
    return -1;
}

int is_target_up_to_date(struct rule *rule, struct minimake_context *c)
{
    if (c)
    {
    }
    if (!file_exists(rule->target))
    {
        return 0;
    }

    time_t target_time = get_file_mtime(rule->target);

    for (int i = 0; i < rule->dep_count; i++)
    {
        if (file_exists(rule->dependencies[i]))
        {
            time_t dep_time = get_file_mtime(rule->dependencies[i]);
            if (dep_time > target_time)
            {
                return 0;
            }
        }
    }

    return 1;
}

int is_nothing_to_be_done(struct rule *rule, struct minimake_context *c)
{
    if (c)
    {
    }
    if (rule->cmd_count == 0)
    {
        return 1;
    }
    return 0;
}

int build_target(struct minimake_context *c, const char *target_name)
{
    struct rule *rule = find_rule(c, target_name);
    if (rule == NULL)
    {
        if (file_exists(target_name))
        {
            printf("minimake: '%s' is up to date.\n", target_name);
            return 0;
        }
        fprintf(stderr, "minimake: *** No rule to make target '%s'. Stop.\n",
                target_name);
        return 1;
    }

    int up_to_date = is_target_up_to_date(rule, c);
    int nothing_to_do = is_nothing_to_be_done(rule, c);

    if (up_to_date && nothing_to_do)
    {
        printf("minimake: Nothing to be done for '%s'.\n", target_name);
        return 0;
    }
    else if (up_to_date)
    {
        printf("minimake: '%s' is up to date.\n", target_name);
        return 0;
    }
    else if (nothing_to_do && rule->dep_count == 0)
    {
        printf("minimake: Nothing to be done for '%s'.\n", target_name);
        return 0;
    }

    int dep_error = 0;
    for (int i = 0; i < rule->dep_count; i++)
    {
        if (build_target(c, rule->dependencies[i]) != 0)
            dep_error = 1;
    }
    if (dep_error)
        return 1;

    up_to_date = is_target_up_to_date(rule, c);
    if (up_to_date)
    {
        printf("minimake: '%s' is up to date.\n", target_name);
        return 0;
    }
    if (rule->cmd_count > 0)
    {
        for (int i = 0; i < rule->cmd_count; i++)
        {
            int should_log = should_log_command(rule->commands[i]);
            char *expanded_cmd = substitute_variables(rule->commands[i], c);
            if (execute_command(expanded_cmd, should_log) != 0)
            {
                fprintf(stderr, "minimake: *** Stop.\n");
                free(expanded_cmd);
                return 1;
            }
            free(expanded_cmd);
        }
    }

    return 0;
}
