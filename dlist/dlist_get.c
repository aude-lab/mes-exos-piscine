#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

int dlist_get(const struct dlist *list, size_t index)
{
    if (!list || index > list->size)
        return -1;

    struct dlist_item *current = list->head;
    size_t i = 0;
    while (current && i < index)
    {
        current = current->next;
        i++;
    }
    return current->data;
}
