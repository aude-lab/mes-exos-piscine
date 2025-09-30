#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>

#define BUFFER_SIZE 1024

char rotx_char(char c, int x) {
    if (c >= 'a' && c <= 'z') {
        int rotation = (c - 'a' + x) % 26;
        if (rotation < 0) rotation += 26;
        return 'a' + rotation;
    } else if (c >= 'A' && c <= 'Z') {
        int rotation = (c - 'A' + x) % 26;
        if (rotation < 0) rotation += 26;
        return 'A' + rotation;
    } else if (c >= '0' && c <= '9') {
        int rotation = (c - '0' + x) % 10;
        if (rotation < 0) rotation += 10;
        return '0' + rotation;
    } else {
        return c;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 0;
    }
    int x = atoi(argv[1]);
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            buffer[i] = rotx_char(buffer[i], x);
        }

        if (write(STDOUT_FILENO, buffer, bytes_read) == -1) {
            err(1, "write failed");
        }
    }

    if (bytes_read == -1) {
        err(1, "read failed");
    }

    return 0;
}
