#include "heap.h"
#include <stdlib.h>
#include <err.h>

static void heapify_up(struct heap *heap, size_t index)
{
    while (index > 0) {
        size_t parent = (index - 1) / 2;
        if (heap->array[parent] >= heap->array[index])
            break;
        int temp = heap->array[parent];
        heap->array[parent] = heap->array[index];
        heap->array[index] = temp;
        
        index = parent;
    }
}

void add(struct heap *heap, int val)
{
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        int *new_array = realloc(heap->array, heap->capacity * sizeof(int));
        if (!new_array)
            err(1, "realloc failed");
        heap->array = new_array;
    }
    heap->array[heap->size] = val;
    heap->size++;
    heapify_up(heap, heap->size - 1);
}
