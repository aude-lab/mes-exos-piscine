#include "allocator.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
struct blk_allocator *blka_new(void)
{
    struct blk_allocator *allocation = malloc(sizeof(struct blk_allocator));
    if (allocation == NULL)
        return NULL;

    allocation->meta = NULL;
    return allocation;
}

struct blk_meta *blka_alloc(struct blk_allocator *blka, size_t size)
{
    int size_1 = sysconf(_SC_PAGESIZE);
    if (size_1 <= 0)
        return NULL;
    if (size == 0)
        size = size_1;

    size_t size_total = ((size + size_1 - 1) / size_1) * size_1;
    void *addr = mmap(NULL, size_total, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (addr == MAP_FAILED)
        return NULL;

    struct blk_meta *address2 = addr;
    address2->size = size_total - sizeof(struct blk_meta);
    address2->next = blka->meta;
    blka->meta = address2;
    return address2;
}

void blka_free(struct blk_meta *blk)
{
    if (blk != NULL)
    {
        void *ptr = blk;
        munmap(ptr, blk->size);
    }
}
void blka_pop(struct blk_allocator *blka)
{
    if (blka->meta == NULL)
        return;
    else
    {
        struct blk_meta *first = blka->meta;
        blka->meta = blka->meta->next;

        blka_free(first);
    }
}
void blka_delete(struct blk_allocator *blka)
{
    while (blka->meta != NULL)
    {
        blka_pop(blka);
    }
    free(blka);
}
