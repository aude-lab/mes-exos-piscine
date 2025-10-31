#include "heap.h"
#include <stdlib.h>

void delete_heap(struct heap *heap)
{
    if (heap) {
        free(heap->array);
        free(heap);
    }
}
