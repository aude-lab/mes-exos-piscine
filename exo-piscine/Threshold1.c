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

size_t dlist_size(const struct dlist *list)
{
    if (!list)
        return 0;
    return list->size;
}
