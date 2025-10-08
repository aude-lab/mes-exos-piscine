#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

int dlist_push_front(struct dlist *list, int element)
{
    if (element < 0)
        return 0;
    struct dlist_item *new = malloc(sizeof(struct dlist_item));
    if (!new)
        return 0;

    new->data = element;
    new->prev = NULL;
    new->next = list->head;

    if (list->head)
        list->head->prev = new;
    else
        list->tail = new;
    list->head = new;
    list->size++;
    return 1;
}
