#include <stddef.h>
#include <stdio.h>

#include "functional_programming.h"

int foldl(int *array, size_t len, int (*func)(int, int))
{
    int accumulator = 0;
    for (size_t i = 0; i < len; i++)
    {
        accumulator = func(accumulator, array[i]);
    }
    return accumulator;
}
