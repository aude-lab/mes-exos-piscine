#include "tests.h"

#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "libstream.h"

/*
** FIXME: This test does not ensure that all of your code
** works properly. You have to add more tests on your own!
*/
Test(basics, basic_open_read_close)
{
    const char *path = "tests/my_file.txt";

    size_t file_length = 0;
    size_t file_index = 0;
    char c = 0;

    // Read the input file without tinylibstream
    char *file_content = get_file_content(path, &file_length);
    if (!file_content)
        cr_fatal("Unable to read the file");

    if (file_length <= LBS_BUFFER_SIZE || file_length > LBS_BUFFER_SIZE * 2)
        cr_fatal("This test cannot handle the given file length");

    // Open the file as reading
    struct stream *stream = lbs_fopen(path, "r");
    const struct stream initialized_stream =
        get_correct_stream(INITIALIZATION, file_content, file_length);
    cr_assert(cr_stream_eq(stream, initialized_stream),
              "Stream content is incorrect after initialization");

    // Read buffer size characters
    for (size_t i = 0; i < LBS_BUFFER_SIZE; i++, file_index++)
    {
        c = lbs_fgetc(stream);
        cr_assert(eq(chr, c, file_content[file_index]));
    }

    // Verify the file descriptor position
    int cursor_pos = lseek(stream->fd, 0, SEEK_CUR);
    cr_assert(eq(int, cursor_pos, LBS_BUFFER_SIZE));

    // Verify stream content
    const struct stream stream_before_buffer_refill =
        get_correct_stream(BEFORE_BUFFER_REFILL, file_content, file_length);
    cr_assert(cr_stream_eq(stream, stream_before_buffer_refill),
              "Stream content before buffer refill is incorrect");

    // Buffer has been fully read, read(2) should be called to refill it
    c = lbs_fgetc(stream);
    cr_assert(eq(chr, c, file_content[file_index++]));

    // Verify stream content
    const struct stream stream_after_buffer_refill =
        get_correct_stream(AFTER_BUFFER_REFILL, file_content, file_length);
    cr_assert(cr_stream_eq(stream, stream_after_buffer_refill),
              "Stream content after buffer refill is incorrect");

    // Read all characters stored in the buffer
    for (size_t j = 1; j < stream->buffered_size; j++, file_index++)
    {
        c = lbs_fgetc(stream);
        cr_assert(eq(chr, c, file_content[file_index]));
    }

    const struct stream ending_stream =
        get_correct_stream(END, file_content, file_length);
    cr_assert(cr_stream_eq(stream, ending_stream),
              "Stream content is incorrect after the file being fully read");

    // Verify the file descriptor position
    cursor_pos = lseek(stream->fd, 0, SEEK_CUR);
    int expected_pos = LBS_BUFFER_SIZE + ending_stream.buffered_size;
    cr_assert(eq(int, cursor_pos, expected_pos));

    // Verify EOF
    char eof = lbs_fgetc(stream);
    cr_assert(eof == LBS_EOF, "Expected: EOF | Got: Not EOF");

    lbs_fclose(stream);
    free(file_content);
}
