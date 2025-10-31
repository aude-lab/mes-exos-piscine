#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

int dlist_push_back(struct dlist *list, int element)
{
    if (element < 0 || !list)
        return 0;

    struct dlist_item *new = malloc(sizeof(struct dlist_item));
    new->data = element;
    new->next = NULL;
    new->prev = list->tail;

    if (list->tail)
        list->tail->next = new;
    else
        list->head = new;

    list->tail = new;

    list->size++;
    return 1;
}
