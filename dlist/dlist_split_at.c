#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

struct dlist *dlist_split_at(struct dlist *list, size_t index)
{
    if (!list || index > list->size)
        return NULL;

    if (index == 0)
    {
        struct dlist *new_list = dlist_init();
        if (new_list == NULL)
            return NULL;
        new_list->head = list->head;
        new_list->tail = list->tail;
        new_list->size = list->size;

        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
        return new_list;
    }

    if (index == list->size)
        return dlist_init();

    struct dlist_item *current = list->head;
    for (size_t i = 0; i < index; i++)
    {
        current = current->next;
    }
    struct dlist *new_list = dlist_init();
    if (new_list == NULL)
        return NULL;
    new_list->head = current;
    new_list->tail = list->tail;
    new_list->size = list->size - index;
    list->tail = current->prev;
    list->tail->next = NULL;
    list->size = index;
    current->prev = NULL;

    return new_list;
}
