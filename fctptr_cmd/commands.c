#include "commands.h"

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int cmd_help(const char *arg)
{
    if (arg)
    {
	    fprintf(stderr, "fctptr_cmd: help takes no argument\n");
            return 1;
    };
    printf("The available commands are:\n");
    printf("help\n");
    printf("hello\n");
    printf("print string\n");
    printf("exit\n");
    printf("cat file\n");
    return 0;
}

int cmd_hello(const char *arg)
{
    if (arg)
    {
	fprintf(stderr, "fctptr_cmd: hello takes no argument\n");
        return 1;
    };
    printf("hello\n");
    return 0;
}

int cmd_print(const char *arg)
{
    if (!arg)
    {
        fprintf(stderr, "fctptr_cmd: print needs one argument\n");
        return 1;
    }
    printf("%s\n", arg);
    return 0;
}

int cmd_exit(const char *arg)
{
    if (arg)
    {
	fprintf(stderr, "fctptr_cmd: exit takes no argument\n");
        return 1;
    };
    exit(0);
}

int cmd_cat(const char *arg)
{
    if (!arg)
    {
        fprintf(stderr, "fctptr_cmd: cat needs one argument\n");
        return 1;
    }


    free(temp_arg);

    int fd = open(arg, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "fctptr_cmd: cannot open file '%s'\n", arg);
        return 1;
    }

    char buffer[1024];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    close(fd);
    return 0;
}

const struct cmd commands[] = { { cmd_help, "help" },
                                { cmd_hello, "hello" },
                                { cmd_print, "print" },
                                { cmd_exit, "exit" },
                                { cmd_cat, "cat" } };
size_t get_commands_count(void)
{
    return sizeof(commands) / sizeof(commands[0]);
}
