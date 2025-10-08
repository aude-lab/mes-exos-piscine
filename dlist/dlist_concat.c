#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

void dlist_concat(struct dlist *list1, struct dlist *list2)
{
    if (!list1)
        return;
    if (!list2 || !list2->head)
        return;

    if (!list1->head)
    {
        list1->head = list2->head;
        list1->tail = list2->tail;
        list1->size = list2->size;
    }
    else
    {
        list1->tail->next = list2->head;
        list2->head->prev = list1->tail;
        list1->tail = list2->tail;
        list1->size += list2->size;
    }
    list2->head = NULL;
    list2->tail = NULL;
    list2->size = 0;
}
