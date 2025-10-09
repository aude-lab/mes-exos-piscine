#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../include/libstream.h"

static int convert_mode_to_flags(const char *mode)
{
    if (strcmp(mode, "r") == 0)
        return O_RDONLY;
    if (strcmp(mode, "r+") == 0)
        return O_RDWR;
    if (strcmp(mode, "w") == 0)
        return O_WRONLY | O_CREAT | O_TRUNC;
    if (strcmp(mode, "w+") == 0)
        return O_RDWR | O_CREAT | O_TRUNC;
    return -1;
}

static enum stream_buffering get_default_buffering(int fd)
{
    if (isatty(fd))
        return STREAM_LINE_BUFFERED;
    return STREAM_BUFFERED;
}

struct stream *lbs_fdopen(int fd, const char *mode)
{
    int theflags = convert_mode_to_flags(mode);
    if (theflags == -1)
        return NULL;

    if (isatty(fd) == -1)
        return NULL;

    struct stream *stream = malloc(sizeof(struct stream));
    if (!stream)
        return NULL;

    stream->flags = theflags;
    stream->error = 0;
    stream->fd = fd;
    stream->io_operation = STREAM_READING;
    stream->buffering_mode = get_default_buffering(fd);
    stream->buffered_size = 0;
    stream->already_read = 0;
    memset(stream->buffer, 0, LBS_BUFFER_SIZE);

    return stream;
}

struct stream *lbs_fopen(const char *path, const char *mode)
{
    int theflags = convert_mode_to_flags(mode);
    if (theflags == -1)
        return NULL;

    int filed = open(path, theflags, 0644);
    if (filed == -1)
        return NULL;

    return lbs_fdopen(filed, mode);
}

int lbs_fflush(struct stream *stream)
{
    if (!stream)
        return -1;

    if (stream->io_operation == STREAM_WRITING && stream->buffered_size > 0)
    {
        ssize_t writen =
            write(stream->fd, stream->buffer, stream->buffered_size);
        if (writen != (ssize_t)stream->buffered_size)
        {
            stream->error = 1;
            return -1;
        }
        stream->buffered_size = 0;
    }
    else if (stream->io_operation == STREAM_READING
             && stream->already_read < stream->buffered_size)
    {
        off_t current_position = lseek(stream->fd, 0, SEEK_CUR);
        off_t new_position = lseek(
            stream->fd,
            current_position - (stream->buffered_size - stream->already_read),
            SEEK_SET);
        if (new_position == -1)
        {
            stream->error = 1;
            return -1;
        }
        stream->buffered_size = 0;
        stream->already_read = 0;
    }

    return 0;
}

int lbs_fclose(struct stream *stream)
{
    if (!stream)
        return -1;

    int result = 0;

    if (lbs_fflush(stream) == -1)
        result = -1;

    if (close(stream->fd) == -1)
        result = -1;

    free(stream);
    return result;
}

static int switchingtowriting(struct stream *stream)
{
    if (stream->io_operation == STREAM_READING)
    {
        if (lbs_fflush(stream) == -1)
            return -1;
        stream->io_operation = STREAM_WRITING;
    }
    return 0;
}

static int flushifn(struct stream *stream)
{
    if (stream->buffering_mode == STREAM_UNBUFFERED)
        return lbs_fflush(stream);

    if (stream->buffered_size == LBS_BUFFER_SIZE)
        return lbs_fflush(stream);

    return 0;
}

int lbs_fputc(int c, struct stream *stream)
{
    if (!stream || !stream_writable(stream))
    {
        if (stream)
            stream->error = 1;
        return -1;
    }

    if (switchingtowriting(stream) == -1)
    {
        stream->error = 1;
        return -1;
    }

    if (flushifn(stream) == -1)
    {
        stream->error = 1;
        return -1;
    }

    stream->buffer[stream->buffered_size++] = (char)c;

    if (stream->buffering_mode == STREAM_LINE_BUFFERED && c == '\n')
    {
        if (lbs_fflush(stream) == -1)
        {
            stream->error = 1;
            return -1;
        }
    }

    return c;
}

static int refill_buffer(struct stream *stream)
{
    if (stream->io_operation != STREAM_READING)
    {
        if (lbs_fflush(stream) == -1)
            return -1;
        stream->io_operation = STREAM_READING;
    }

    ssize_t bytes_read = read(stream->fd, stream->buffer, LBS_BUFFER_SIZE);
    if (bytes_read == -1)
    {
        stream->error = 1;
        return -1;
    }
    else if (bytes_read == 0)
    {
        return LBS_EOF;
    }

    stream->buffered_size = bytes_read;
    stream->already_read = 0;
    return 0;
}

int lbs_fgetc(struct stream *stream)
{
    if (!stream || !stream_readable(stream))
    {
        if (stream)
            stream->error = 1;
        return -1;
    }

    if (stream_remaining_buffered(stream) == 0)
    {
        int result = refill_buffer(stream);
        if (result == -1 || result == LBS_EOF)
        {
            stream->error = (result == -1) ? 1 : 0;
            return LBS_EOF;
        }
    }

    unsigned char c = stream->buffer[stream->already_read++];
    return c;
}
