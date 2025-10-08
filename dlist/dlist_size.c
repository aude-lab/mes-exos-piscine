#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

size_t dlist_size(const struct dlist *list)
{
    if (!list)
        return 0;
    return list->size;
}
