#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

struct dlist *dlist_init(void)
{
    struct dlist *new_dlist = malloc(sizeof(struct dlist));
    new_dlist->size = 0;
    new_dlist->head = NULL;
    new_dlist->tail = NULL;
    return new_dlist;
}
