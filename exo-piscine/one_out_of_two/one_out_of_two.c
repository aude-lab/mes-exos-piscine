#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        return 1;
    char c;
    ssize_t bread;
    int skip_next = 0;

    while ((bread = read(fd, &c, 1)) > 0)
    {
        if (!skip_next)
        {
            if (write(STDOUT_FILENO, &c, 1) == -1)
            {
                close(fd);
                return 1;
            }
        }
        skip_next = !skip_next;
    }
    if (bread == -1)
    {
        close(fd);
        return 1;
    }
    if (close(fd) == -1)
    {
        return 1;
    }

    return 0;
}
