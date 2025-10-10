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
    if (mode == NULL)
        return -1;
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

/*static enum stream_buffering get_default_buffering(int fd)
{
    if (isatty(fd))
        return STREAM_LINE_BUFFERED;
    return STREAM_BUFFERED;
}*/

struct stream *lbs_fdopen(int fd, const char *mode)
{
    int theflags = convert_mode_to_flags(mode);
    if (theflags == -1 || fd < 0)
        return NULL;

    if (lseek(fd, 0, SEEK_CUR) == -1 && errno == EBADF)
        return NULL;

    struct stream *stream = malloc(sizeof(struct stream));
    if (!stream)
        return NULL;

    stream->flags = theflags;
    stream->error = 0;
    stream->fd = fd;
    stream->io_operation = STREAM_READING;
    // stream->buffering_mode = get_default_buffering(fd);
    stream->buffered_size = 0;
    stream->already_read = 0;
    memset(stream->buffer, 0, LBS_BUFFER_SIZE);

    stream->buffering_mode =
        isatty(fd) ? STREAM_LINE_BUFFERED : STREAM_BUFFERED;

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

    struct stream *stream = lbs_fdopen(filed, mode);
    if (!stream)
    {
        close(filed);
    }
    return stream;
}

int lbs_fflush(struct stream *stream)
{
    if (!stream)
        return -1;

    if (stream->io_operation == STREAM_WRITING && stream->buffered_size > 0)
    {
        size_t total = 0;
        while (total < stream->buffered_size)
        {
            ssize_t written = write(stream->fd, stream->buffer + total,
                                    stream->buffered_size - total);
            if (written == -1)
            {
                stream->error = 1;
                return -1;
            }
            total += written;
        }
        stream->buffered_size = 0;
        return 0;
    }

    if (stream->io_operation == STREAM_READING
        && stream->already_read < stream->buffered_size)
    {
        size_t unread = stream->buffered_size - stream->already_read;

        if (lseek(stream->fd, -(off_t)unread, SEEK_CUR) == -1)
        {
            stream->error = 1;
            return -1;
        }

        memset(stream->buffer, 0, LBS_BUFFER_SIZE);
        stream->buffered_size = 0;
        stream->already_read = 0;
        stream->io_operation = STREAM_READING;
        return 0;
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
        stream->buffered_size = 0;
        stream->already_read = 0;
    }
    return 0;
}

static int switchingtoreading(struct stream *stream)
{
    if (stream->io_operation == STREAM_WRITING)
    {
        if (lbs_fflush(stream) == -1)
            return -1;
        stream->io_operation = STREAM_READING;
        stream->buffered_size = 0;
        stream->already_read = 0;
    }
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

    if (stream->buffering_mode == STREAM_UNBUFFERED)
    {
        char ch = c;
        ssize_t written = write(stream->fd, &ch, 1);
        if (written != 1)
        {
            stream->error = 1;
            return -1;
        }
        return c;
    }

    int should_flush = 0;

    if (stream->buffered_size == LBS_BUFFER_SIZE)
        should_flush = 1;

    else if (stream->buffering_mode == STREAM_LINE_BUFFERED)
    {
        if (stream->buffered_size >= LBS_BUFFER_SIZE - 1 || c == '\n')
            should_flush = 1;
    }
    if (should_flush && lbs_fflush(stream) == -1)
    {
        stream->error = 1;
        return -1;
    }

    stream->buffer[stream->buffered_size++] = c;

    if ((stream->buffering_mode == STREAM_UNBUFFERED)
        || (stream->buffering_mode == STREAM_LINE_BUFFERED && c == '\n')
        || (stream->buffered_size == LBS_BUFFER_SIZE))
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

    if (switchingtoreading(stream) == -1)
    {
        stream->error = 1;
        return -1;
    }

    if (stream->buffered_size == 0
        || stream->already_read >= stream->buffered_size)
    {
        int result = refill_buffer(stream);
        if (result == -1)
        {
            stream->error = 1;
            return -1;
        }
        if (result == LBS_EOF)
        {
            return LBS_EOF;
        }
    }

    unsigned char c = stream->buffer[stream->already_read++];
    return c;
}
