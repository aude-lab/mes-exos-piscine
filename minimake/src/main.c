#define _POSIX_C_SOURCE 200809L
#include "main.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "parser.h"

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

struct rule *find_default_target(struct minimake_context *ctx)
{
    for (int i = 0; i < ctx->rule_count; i++)
    {
        if (strchr(ctx->rules[i]->target, '%') == NULL)
        {
            return ctx->rules[i];
        }
    }
    return NULL;
}

int execute_command(char *command, int should_log)
{
    char *cmd_to_execute = command;

    if (*command == '@')
    {
        cmd_to_execute = command + 1;
        while (*cmd_to_execute && isspace(*cmd_to_execute))
        {
            cmd_to_execute++;
        }
    }
    if (should_log)
    {
        printf("%s\n", command);
        fflush(stdout);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }
    else if (pid == 0)
    {
        char *argv[] = { "/bin/sh", "-c", cmd_to_execute, NULL };
        char *envp[] = { NULL };

        execve("/bin/sh", argv, envp);
        perror("execve");
        exit(127);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status);
        }
        else
        {
            return 1;
        }
    }

}

int should_log_command(const char *command)
{
    while (*command && isspace(*command))
    {
        command++;
    }
    return (*command != '@');
}

void pretty_print(struct minimake_context *c)
{
    if (c->var_count > 0)
    {
        printf("# variables\n");
        for (int i = 0; i < c->var_count; i++)
        {
            printf("'%s' = '%s'\n", c->variables[i]->name,
                   c->variables[i]->value);
        }
    }

    if (c->rule_count > 0)
    {
        printf("# rules\n");
        for (int i = 0; i < c->rule_count; i++)
        {
            struct rule *rule = c->rules[i];
            printf("(%s):", rule->target);

            for (int j = 0; j < rule->dep_count; j++)
                printf(" [%s]", rule->dependencies[j]);
            printf("\n");

            for (int k = 0; k < rule->cmd_count; k++)
            {
                char *cmd = rule->commands[k];
                char normalized_cmd[1024];

                if (cmd[0] == '@')
                {
                    const char *content = cmd + 1;
                    while (*content && isspace(*content) && *content != '\t')
                        content++;
                    if (content > cmd + 1)
                    {
                        snprintf(normalized_cmd, sizeof(normalized_cmd), "@ %s",
                                 content);
                        printf("    '%s'\n", normalized_cmd);
                        continue;
                    }
                }

                printf("    '%s'\n", cmd);
            }
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
        pretty_print(&c);

    else
    {
        if (c.target_count == 0)
        {
            struct rule *default_target = find_default_target(&c);
            if (default_target == NULL)
            {
                fprintf(stderr, "minimake: *** No targets. Stop.\n");
                free_minimake_context(&c);
                return 2;
            }
            build_target(&c, default_target->target);
        }
        else
        {
            for (int i = 0; i < c.target_count; i++)
            {
                build_target(&c, c.targets[i]);
            }
        }
    }

    free_minimake_context(&c);
    return 0;
}
