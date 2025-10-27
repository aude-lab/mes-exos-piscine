#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void init_minimake(struct minimake_context *c)
{
    c->makefile_name = NULL;
    c->pretty_print = 0;
    c->show_help = 0;
    c->targets = NULL;
    c->target_count = 0;
}

void help_option(void)
{
    printf("Use: minimake [options] [targets]\n");
    printf("Options:\n");
    printf("  -f file    Using file as makefile\n");
    printf("  -h         Showing this help message\n");
    printf("  -p         Pretty-printing the makefile\n");
}

void free_minimake_context(struct minimake_context *c)
{
    for (int i = 0; i < c->target_count; i++)
        free(c->targets[i]);
    free(c->targets);
}

int parse_arguments(int argc, char *argv[], struct minimake_context *c)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (strcmp(argv[i], "-f") == 0)
            {
                if (i + 1 >= argc)
                {
                    fprintf(stderr, "minimake: *** missing argument to -f\n");
                    return 1;
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
                return 1;
            }
        }
	else {
        c->target_count++;
        c->targets = realloc(c->targets, c->target_count * sizeof(char *));
        c->targets[c->target_count - 1] = mystrdup(argv[i]);
    }
    }

    return 0;
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

    printf("pas encore géré");
    free_minimake_context(&c);
    return 0;
}
