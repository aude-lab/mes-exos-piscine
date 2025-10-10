int cmd_cat(const char *arg)
{
    if (!arg)
    {
        fprintf(stderr, "fctptr_cmd: cat needs one argument\n");
        return 1;
    }

    int fd = open(arg, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "fctptr_cmd: cannot open file ");
        perror(arg);
        return 1;
    }

    char buffer[1024];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read)
        {
            perror("write");
            close(fd);
            return 1;
        }
    }

    if (bytes_read == -1)
    {
        perror("read");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
