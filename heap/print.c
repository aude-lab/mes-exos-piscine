#include "heap.h"
#include <stdio.h>

static void print_preorder(const struct heap *heap, size_t index)
{
    if (index >= heap->size)
        return;
    printf("%d", heap->array[index]);
    if (2 * index + 1 < heap->size) {
        printf(" ");
        print_preorder(heap, 2 * index + 1);
    }
    if (2 * index + 2 < heap->size) {
        printf(" ");
        print_preorder(heap, 2 * index + 2);
    }
}

void print_heap(const struct heap *heap)
{
    if (heap->size == 0) {
        printf("\n");
        return;
    }
    print_preorder(heap, 0);
    printf("\n");
}
