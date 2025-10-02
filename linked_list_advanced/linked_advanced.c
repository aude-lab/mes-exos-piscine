#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct list *list_prepend(struct list *list, int value)
{
    struct list *new = malloc(sizeof(struct list));
    if (!new)
        return NULL;
    new->data = value;
    new->next = list;
    return new;
}

size_t list_length(struct list *list)
{
    size_t len = 0;
    struct list *current = list;
    
    while (current)
    {
        len++;
        current = current->next;
    }
    return len;
}

void list_print(struct list *list)
{
    struct list *current = list;
    int first = 1;
    
    while (current)
    {
        if (!first)
            printf(" ");
        printf("%d", current->data);
        first = 0;
        current = current->next;
    }
    
    if (!first)
        printf("\n");
}

void list_destroy(struct list *list)
{
    struct list *current = list;
    
    while (current)
    {
        struct list *next = current->next;
        free(current);
        current = next;
    }
}

struct list *list_append(struct list *list, int value)
{
    struct list *new = malloc(sizeof(struct list));
    if (!new)
        return NULL;
    new->data = value;
    new->next = NULL;
    
    if (!list)
        return new;
    
    struct list *current = list;
    while (current->next)
        current = current->next;
    
    current->next = new;
    return list;
}
