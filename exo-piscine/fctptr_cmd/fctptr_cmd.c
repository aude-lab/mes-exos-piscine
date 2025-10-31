#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

#define BUFFER_SIZE 1024



static char *read_command_line(char *buffer, size_t buffer_size)
{
    printf("cmd$ ");
    fflush(stdout);

    if (!fgets(buffer, buffer_size, stdin))
        return NULL;

    size_t mylen = strlen(buffer);
    if (mylen > 0 && buffer[mylen - 1] == '\n')
    {
        buffer[--mylen] = '\0';
    }
    return buffer;
}

static int parse_command(char *input, char **command, char **arg)
{
    if (strlen(input) == 0)
        return 0; 

    *command = strtok(input, " ");
    *arg = strtok(NULL, "\n");
    if (*arg && strlen(*arg) == 0)
        *arg = NULL;
    return 1; 
}

static void execute_command(char *command, char *arg)
{
    int founded = 0;
    for (size_t i = 0; i < get_commands_count(); i++)
    {
        if (strcmp(command, commands[i].command_name) != 0)
            continue;

        founded = 1;
        commands[i].handle(arg);
        break;
    }

    if (!founded)
        fprintf(stderr, "fctptr_cmd: unknown command\n");
}

int main(void)
{
    char buffer[BUFFER_SIZE];
    char *command_line;
    char *command;
    char *arg;

    while (1)
    {
        command_line = read_command_line(buffer, sizeof(buffer));
        if (!command_line)
            break; 

        if (!parse_command(command_line, &command, &arg))
            continue; 

        execute_command(command, arg);
    }
    return 0;
}
















/*int main(void)
{
    char new_buffer[BUFFER_SIZE];

    while (1)
    {
        printf("cmd$ ");
        fflush(stdout);

        if (!fgets(new_buffer, sizeof(new_buffer), stdin))
            break;

        size_t mylen = strlen(new_buffer);
        if (mylen > 0 && new_buffer[mylen - 1] == '\n')
        {
            new_buffer[--mylen] = '\0';
        }

        if (mylen == 0)
            continue;

        if (mylen > 0 && new_buffer[mylen - 1] == '$')
            new_buffer[--mylen] = '\0';
	if (mylen == 0) continue;

        char *command = strtok(new_buffer, " ");
        char *arg = strtok(NULL, "\n");
        if (arg && strlen(arg) == 0)
            arg = NULL;

        int founded = 0;

        for (size_t i = 0; i < get_commands_count(); i++)
        {
            if (strcmp(command, commands[i].command_name))
                continue;

            founded = 1;
            if ((!strcmp(command, "print") || !strcmp(command, "cat")) && !arg)
            {
                fprintf(stderr, "fctptr_cmd: %s can take only one argument\n",
                        command);
            }
            else if (!strcmp(command, "exit") && arg)
            {
                fprintf(stderr, "fctptr_cmd: %s takes no argument\n", command);
            }
            else
                commands[i].handle(arg);
            break;
        }

        if (!founded)
            fprintf(stderr, "fctptr_cmd: unknown command\n");
    }
    return 0;
}*/
