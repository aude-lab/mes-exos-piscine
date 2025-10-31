#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "tests.h"

/*
** Display the given error message and always return NULL.
*/
static char *error_file(const char *msg)
{
    warnx("%s", msg);
    return NULL;
}

char *get_file_content(const char *path, size_t *content_length)
{
    struct stat st;
    if (stat(path, &st) == -1)
        return error_file("Unable to get the file length");
    size_t size = st.st_size;

    FILE *file = fopen(path, "r");
    if (!file)
        return error_file("Unable to open the file");

    char *content = malloc(sizeof(char) * (size + 1));
    if (!content)
        return error_file("Unable to allocate memory while reading the file");

    size_t nb_read = fread(content, 1, size, file);
    while (nb_read != 0 && nb_read < size)
    {
        nb_read += fread(content + nb_read, 1, size - nb_read, file);
    }
    if (nb_read != size)
        return error_file("Unable to fully read the file");

    content[size] = '\0';

    if (content_length)
        *content_length = size;

    return content;
}

struct stream get_correct_stream(enum step step, char *file_content,
                                 size_t file_length)
{
    struct stream stream = { .flags = O_RDONLY,
                             .error = 0,
                             .fd = 0,
                             .io_operation = STREAM_READING,
                             .buffering_mode = STREAM_BUFFERED,
                             .buffer = { 0 } };
    switch (step)
    {
    case INITIALIZATION:
        stream.buffered_size = 0;
        stream.already_read = 0;
        break;
    case BEFORE_BUFFER_REFILL:
        stream.buffered_size = LBS_BUFFER_SIZE;
        stream.already_read = LBS_BUFFER_SIZE;
        memcpy(stream.buffer, file_content, LBS_BUFFER_SIZE);
        break;
    case AFTER_BUFFER_REFILL:
        stream.buffered_size = file_length - LBS_BUFFER_SIZE;
        stream.already_read = 1;
        memcpy(stream.buffer, file_content + LBS_BUFFER_SIZE,
               file_length - LBS_BUFFER_SIZE);
        break;
    case END:
        stream.buffered_size = file_length - LBS_BUFFER_SIZE;
        stream.already_read = file_length - LBS_BUFFER_SIZE;
        memcpy(stream.buffer, file_content + LBS_BUFFER_SIZE,
               file_length - LBS_BUFFER_SIZE);
        break;
    }
    return stream;
}

int cr_stream_eq(struct stream *stream, struct stream expected)
{
    return stream != NULL && stream->flags == expected.flags
        && stream->error == expected.error && stream->fd > expected.fd
        && stream->io_operation == expected.io_operation
        && stream->buffering_mode == expected.buffering_mode
        && stream->buffered_size == expected.buffered_size
        && stream->already_read == expected.already_read
        && !memcmp(stream->buffer, expected.buffer, stream->buffered_size);
}
