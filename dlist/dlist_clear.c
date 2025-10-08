#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

void dlist_clear(struct dlist *list)
{
    if (list == NULL)
        return;
    while (list->head != NULL)
    {
        struct dlist_item *first = list->head;
        list->head = first->next;
        free(first);
    }
    list->tail = NULL;
    list->size = 0;
}
