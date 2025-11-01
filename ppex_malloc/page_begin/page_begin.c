#include "page_begin.h"

#include <stdio.h>
void *page_begin(void *ptr, size_t page_size)
{
    if ((page_size & (page_size - 1)) != 0)
        return NULL;
    if (page_size < 16 || 2147483648 < page_size)
        return NULL;
    char *ptr2 = ptr;
    char *ptr3 = NULL;
    size_t t = ptr2 - ptr3;
    size_t resultat = t & ~(page_size - 1);
    return (void *)resultat;
}
