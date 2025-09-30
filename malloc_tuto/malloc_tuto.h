#ifndef MALLOC_TUTO_H
#define MALLOC_TUTO_H

#include <stddef.h>

int *create_array(size_t size);
void free_array(int *arr);
void read_and_inc(int *v);
char *my_strdup(const char *str);
char *my_strndup(const char *str, size_t n);

#endif
