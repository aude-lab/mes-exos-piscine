#define _POSIX_C_SOURCE 200809L
#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        errx(1, "no file enter");

    FILE *file = fopen(argv[1], "r");
    if (!file)
        err(1, "fopen failed");

    char *line = NULL;
    size_t len = 0;
    size_t indmessage = 0;
    int countchar = 0;
    char *message = malloc(4096);

    while (getdelim(&line, &len, '\n', file) > 0)
    {
        for (int i = 0; line[i]; i++)
        {
            countchar = (line[i] == ';') ? 0 : countchar + 1;
            if (countchar == 4)
                message[indmessage++] = line[i];
        }
    }

    for (size_t i = 0; i < indmessage; i++)
        putchar(message[i]);
    putchar('\n');

    free(line);
    free(message);
    if (fclose(file) == EOF)
        err(1, "fclose failed");
    return 0;
}
