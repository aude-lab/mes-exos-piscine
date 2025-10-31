#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

int dlist_find(const struct dlist *list, int element)
{
    struct dlist_item *verif = list->head;
    for (size_t i = 0; i < list->size; i++)
    {
        if (verif->data == element)
            return i;
        verif = verif->next;
    }
    return -1;
}
