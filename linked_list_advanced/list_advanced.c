#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct list *list_insert(struct list *list, int value, size_t index)
{
    if (index == 0)
        return list_prepend(list, value);
    
    struct list *new = malloc(sizeof(struct list));
    if (!new)
        return NULL;
    new->data = value;
    
    struct list *current = list;
    size_t i = 0;
    
    while (current && i < index - 1)
    {
        current = current->next;
        i++;
    }
    
    if (!current)
    {
        free(new);
        return list_append(list, value);
    }
    
    new->next = current->next;
    current->next = new;
    return list;
}

struct list *list_remove(struct list *list, size_t index)
{
    if (!list)
        return NULL;
    
    if (index == 0)
    {
        struct list *new_head = list->next;
        free(list);
        return new_head;
    }
    
    struct list *current = list;
    size_t i = 0;
    
    while (current->next && i < index - 1)
    {
        current = current->next;
        i++;
    }
    
    if (!current->next)
        return list;
    
    struct list *to_remove = current->next;
    current->next = to_remove->next;
    free(to_remove);
    return list;
}

int list_find(struct list *list, int value)
{
    struct list *current = list;
    int index = 0;
    
    while (current)
    {
        if (current->data == value)
            return index;
        current = current->next;
        index++;
    }
    
    return -1;
}

struct list *list_concat(struct list *list, struct list *list2)
{
    if (!list)
        return list2;
    
    struct list *current = list;
    while (current->next)
        current = current->next;
    
    current->next = list2;
    return list;
}
