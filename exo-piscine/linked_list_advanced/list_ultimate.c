#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct list *list_sort(struct list *list)
{
    if (!list || !list->next)
        return list;
    
    // Tri par insertion (simple à implémenter)
    struct list *sorted = NULL;
    struct list *current = list;
    
    while (current)
    {
        struct list *next = current->next;
        
        // Insérer current dans sorted à la bonne position
        if (!sorted || current->data <= sorted->data)
        {
            current->next = sorted;
            sorted = current;
        }
        else
        {
            struct list *temp = sorted;
            while (temp->next && temp->next->data < current->data)
                temp = temp->next;
            
            current->next = temp->next;
            temp->next = current;
        }
        
        current = next;
    }
    
    return sorted;
}

struct list *list_reverse(struct list *list)
{
    struct list *prev = NULL;
    struct list *current = list;
    struct list *next = NULL;
    
    while (current)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    
    return prev;
}

struct list *list_split(struct list *list, size_t index)
{
    if (!list)
        return NULL;
    
    if (index == 0)
    {
        struct list *second = list;
        return second;
    }
    
    struct list *current = list;
    size_t i = 0;
    
    while (current && i < index - 1)
    {
        current = current->next;
        i++;
    }
    
    if (!current || !current->next)
        return NULL;
    
    struct list *second = current->next;
    current->next = NULL;
    return second;
}
