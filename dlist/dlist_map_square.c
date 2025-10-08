#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

void dlist_map_square(struct dlist *list)
{
    struct dlist_item *curr = list->head;
    while (curr != NULL)
    {
        curr->data = curr->data * curr->data;
        curr = curr->next;
    }
}
