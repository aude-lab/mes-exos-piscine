#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
int create_file_755(const char *path)
{
    int fd = open(path, O_CREAT | O_EXCL | O_WRONLY, 0755);

    if (fd == -1)
        return 1;
    if (close(fd) == -1)
        return 1;
    return 0;
}
