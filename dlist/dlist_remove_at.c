#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

int dlist_remove_at(struct dlist *list, size_t index)
{
    if (!list || index >= list->size)
        return -1;

    struct dlist_item *aremove;
    int value;

    if (index == 0)
    {
        aremove = list->head;
        list->head = aremove->next;

        if (list->head)
            list->head->prev = NULL;
        else
            list->tail = NULL;
    }

    else if (index == list->size - 1)
    {
        aremove = list->tail;
        list->tail = aremove->prev;
        list->tail->next = NULL;
    }

    else
    {
        aremove = list->head;
        for (size_t i = 0; i < index; i++)
        {
            aremove = aremove->next;
        }

        aremove->prev->next = aremove->next;
        aremove->next->prev = aremove->prev;
    }
    value = aremove->data;
    free(aremove);
    list->size--;

    return value;
}
