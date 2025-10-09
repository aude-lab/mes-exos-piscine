#ifndef TESTS_H
#define TESTS_H

#include <stddef.h>

#include "libstream.h"

enum step
{
    INITIALIZATION,
    BEFORE_BUFFER_REFILL,
    AFTER_BUFFER_REFILL,
    END,
};

/*
** Read the full content of the file located at the given path.
** Set the content_length argument to the length of the returned string.
**
** If any errors occurs, NULL is returned.
*/
char *get_file_content(const char *path, size_t *content_length);

/*
** Return the expected stream struct after each reading steps.
**
** Warning: This function is only correct for files that have between
** LBS_BUFFER_SIZE and 2*LBS_BUFFER_SIZE characters.
*/
struct stream get_correct_stream(enum step step, char *file_content,
                                 size_t file_length);

/*
** Check that the provided stream is consistent with the reference stream.
** Some fields must be stricly equal.
*/
int cr_stream_eq(struct stream *stream, struct stream expected);

#endif /* ! TESTS_H */
