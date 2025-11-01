#include <stddef.h>
#include <stdio.h>

void *beware_overflow(void *ptr, size_t nmemb, size_t size)
{
    char *castp = ptr;
    size_t val;
    if (__builtin_mul_overflow(size, nmemb, &val))
    {
        return NULL;
    }
    else
    {
        ptr = castp + val;
        return ptr;
    }
}
