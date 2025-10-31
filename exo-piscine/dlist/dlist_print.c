#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"
void dlist_print(const struct dlist *list)
{
    if (list->size == 0)
        return;
    struct dlist_item *cur = list->head;
    while (cur != NULL)
    {
        printf("%d\n", cur->data);
        cur = cur->next;
    }
}
