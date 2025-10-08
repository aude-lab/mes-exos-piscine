#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

int dlist_insert_at(struct dlist *list, int element, size_t index)
{
    if (element < 0 || index > list->size || !list)
        return 0;

    if (index == 0)
        return dlist_push_front(list, element);

    if (index == list->size)
        return dlist_push_back(list, element);

    struct dlist_item *new = malloc(sizeof(struct dlist_item));
    if (!new)
        return 0;
    new->data = element;

    struct dlist_item *previ = list->head;
    for (size_t i = 0; i < index; i++)
    {
        previ = previ->next;
    }

    new->prev = previ->prev;
    new->next = previ;

    previ->prev->next = new;

    previ->prev = new;
    list->size++;
    return 1;
}
