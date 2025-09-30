#include "heap.h"
#include <stdlib.h>
#include <err.h>

struct heap *create_heap(void)
{
	struct heap *new = malloc(sizeof(struct heap));
	if (!new)
		return NULL;
	new->capacity = 8;
    	new->size = 0;
    	new->array = malloc(new->capacity * sizeof(int));
    	if (!new->array) {
        	free(new);
        	return NULL;
    	}

    	return new;
}
