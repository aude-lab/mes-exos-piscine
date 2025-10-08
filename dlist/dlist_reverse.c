#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

void dlist_reverse(struct dlist *list)
{
    if (list->head == NULL)
        return;
    struct dlist_item *curr = list->head;
    struct dlist_item *tem = NULL;

    list->head = list->tail;
    list->tail = curr;
    while (curr != NULL)
    {
        tem = curr->prev;
        curr->prev = curr->next;
        curr->next = tem;
        curr = curr->prev;
    }
}
