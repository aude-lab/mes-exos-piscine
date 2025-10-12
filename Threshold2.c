#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

int dlist_get(const struct dlist *list, size_t index)
{
    if (!list || index > list->size)
        return -1;

    struct dlist_item *current = list->head;
    size_t i = 0;
    while (current && i < index)
    {
        current = current->next;
        i++;
    }
    return current->data;
}

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

int dlist_find(const struct dlist *list, int element)
{
    struct dlist_item *verif = list->head;
    for (size_t i = 0; i < list->size; i++)
    {
        if (verif->data == element)
            return i;
        verif = verif->next;
    }
    return -1;
}

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

void dlist_clear(struct dlist *list)
{
    if (list == NULL)
        return;
    while (list->head != NULL)
    {
        struct dlist_item *first = list->head;
        list->head = first->next;
        free(first);
    }
    list->tail = NULL;
    list->size = 0;
}
