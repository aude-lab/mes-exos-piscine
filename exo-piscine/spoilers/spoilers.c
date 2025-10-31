#include <ctype.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        write(STDERR_FILENO, "Invalid number of arguments\n", 28);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        write(STDERR_FILENO, "Could not open file\n", 20);
        return 1;
    }
    off_t sizef = lseek(fd, 0, SEEK_END);
    if (sizef == -1)
        err(1, "lseek");
    char result[2] = { 0 };
    int f = 0;
    off_t pos = sizef - 1;
    while (pos >= 0 && f < 2)
    {
        if (lseek(fd, pos, SEEK_SET) == -1)
        {
            err(1, "lseek");
        }
        char c;
        if (read(fd, &c, 1) != 1)
        {
            err(1, "read");
        }
        if (!isspace(c))
        {
            result[1 - f++] = c;
        }

        pos--;
    }

    if (f < 2)

    {
        write(STDERR_FILENO, "File is not long enough\n", 24);
        close(fd);
        return 1;
    }
    write(STDOUT_FILENO, result, 2);
    write(STDOUT_FILENO, "\n", 1);

    close(fd);
    return 0;
}
