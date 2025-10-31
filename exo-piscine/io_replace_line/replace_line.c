#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int replace_line(const char *file_in, const char *file_out, const char *content, int n)
{
    if (n < 0)
        return -1;

    FILE *filein = fopen(file_in, "r");
    if (filein == NULL)
        return -1;

    FILE *fileout = fopen(file_out, "w");
    if (!fileout)
    {
        fclose(filein);
        return -1;
    }
    char *line = NULL;
    size_t mylen = 0;
    ssize_t read;
    int line_count = 0;
    int replaced = 0;

    while ((read = getline(&line, &mylen, filein)) != -1)
    {
        if (line_count == n)
        {
            fputs(content, fileout);
            replaced = 1;
        }
        else
        {
            fputs(line, fileout);
        }
        line_count++;
    }

    free(line);
    fclose(filein);
    fclose(fileout);
    if (!replaced)
        return 1;

    return 0;
}
