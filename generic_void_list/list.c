#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <err.h>

struct list *list_prepend(struct list *list, const void *value, size_t data_size)
{
    struct list *new = malloc(sizeof(struct list));
    if (!new)
        return NULL;
    new->data = malloc(data_size);
    if (!new->data) 
    {
        free(new);
        return NULL;
    }
    memcpy(new->data, value, data_size);
    new->next = list; 
    return new;
}

size_t list_length(struct list *list)
{
	if (!list)
		return 0;
	size_t length = 0;
	struct list *curr = list;
	while (curr != NULL) {
        	length++;
        	curr = curr->next;
    }

    return length;
}

void list_destroy(struct list *list)
{
	while (list != NULL) {
        	struct list *next = list->next;
        	free(list->data);
        	free(list);      
        	list = next;
    }
}
