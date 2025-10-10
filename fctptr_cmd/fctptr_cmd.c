#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

#define BUFFER_SIZE 1024

int main(void)
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
}
