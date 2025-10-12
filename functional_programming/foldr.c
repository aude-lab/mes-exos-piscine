#include <stddef.h>
#include <stdio.h>

#include "functional_programming.h"

int foldr(int *array, size_t len, int (*func)(int, int))
{
    int accumulator = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        accumulator = func(array[i], accumulator);
    }
    return accumulator;
}
