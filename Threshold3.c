#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

void dlist_map_square(struct dlist *list)
{
    struct dlist_item *curr = list->head;
    while (curr != NULL)
    {
        curr->data = curr->data * curr->data;
        curr = curr->next;
    }
}

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
