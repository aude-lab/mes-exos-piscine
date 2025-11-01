#include "recycler.h"

#include <stdio.h>
#include <stdlib.h>
struct recycler *recycler_create(size_t block_size, size_t total_size)
{
    if (block_size % sizeof(size_t) != 0)
        return NULL;
    else if (block_size == 0 || total_size == 0)
        return NULL;
    else if (total_size % block_size != 0)
        return NULL;
    struct recycler *rcreate = malloc(sizeof(struct recycler));
    if (rcreate == NULL)
        return NULL;
    rcreate->chunk = malloc(total_size);
    if (rcreate->chunk == NULL)
    {
        free(rcreate);
        return NULL;
    }
    rcreate->free = rcreate->chunk;
    rcreate->capacity = total_size / block_size;
    struct free_list *ptr = rcreate->chunk;
    for (size_t i = 0; i < rcreate->capacity - 1; i++)
    {
        void *ptr2 = ptr;
        char *ptr3 = ptr2;
        ptr3 = ptr3 + block_size;
        void *ptr4 = ptr3;
        struct free_list *ptr5 = ptr4;
        ptr = ptr5;
        ptr5 = ptr5->next;
    }

    ptr->next = NULL;

    return rcreate;
}
void recycler_destroy(struct recycler *r)
{
    if (r != NULL)
    {
        free(r->chunk);
        free(r);
    }
}
void *recycler_allocate(struct recycler *r)
{
    if (r == NULL || r->free == NULL)
        return NULL;
    void *allocated = r->free;
    struct free_list *ptr = r->free;
    r->free = ptr->next;
    return allocated;
}
void recycler_free(struct recycler *r, void *block)
{
    if (r != NULL || block != NULL)
    {
        r->chunk = block;
        struct free_list *ptr = block;
        ptr->next = r->free;
        r->free = ptr;
    }
}
