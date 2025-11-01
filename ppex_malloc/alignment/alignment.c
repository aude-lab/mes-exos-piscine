#include "alignment.h"

#include <stddef.h>
#include <stdio.h>
size_t align(size_t size)
{
    size_t aligned = sizeof(long double);
    size_t res;
    if (__builtin_mul_overflow(size, aligned, &res))
        return 0;
    else if (size % aligned == 0)
        return size;
    else
        return ((size + aligned - 1) / aligned) * aligned;
}
