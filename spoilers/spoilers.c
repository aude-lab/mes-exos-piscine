#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid number of arguments\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "Could not open file\n");
        return 1;
    }
    off_t sizef = lseek(fd, 0, SEEK_END);
    if (sizef == -1)
    {
        fprintf(stderr, "error\n");
        close(fd);
        return 1;
    }

    char result[2] = { 0 };
    int f = 0;
    off_t pos = sizef - 1;
    while (pos >= 0 && f < 2)
    {
        if (lseek(fd, pos, SEEK_SET) == -1)
        {
            fprintf(stderr, "error\n");
            close(fd);
            return 1;
        }
        char c;
        if (read(fd, &c, 1) != 1)
        {
            fprintf(stderr, "error\n");
            close(fd);
            return 1;
        }
        if (!isspace(c))
        {
            result[1 - f] = c;
            f++;
        }

        pos--;
    }

    if (f < 2)

    {
        fprintf(stderr, "File is not long enough\n");
        close(fd);
        return 1;
    }
    write(STDOUT_FILENO, result, 2);
    write(STDOUT_FILENO, "\n", 1);

    close(fd);
    return 0;
}
