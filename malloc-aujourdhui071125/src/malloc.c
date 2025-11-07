#include "malloc.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

struct blk *head = NULL;

#define MIN_ALLOC_SIZE (128 * 1024)

static void *block_allocator(size_t size)
{
    size_t page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0)
        return NULL;

    size_t needed = size + sizeof(struct blk);
    size_t alloc_size = needed < MIN_ALLOC_SIZE ? MIN_ALLOC_SIZE : needed;

    size_t size_total = ((alloc_size + page_size - 1) / page_size) * page_size;
    
    struct blk *block = mmap(NULL, size_total, PROT_READ | PROT_WRITE,
                             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED)
        return NULL;
    block->size = size_total - sizeof(struct blk);
    block->freedom = 1;
    block->next = NULL;
    block->prev = NULL;
    block->page_start = block;  
    block->page_size = size_total;
    return block;
}

static size_t align(size_t size)
{
    size_t aligne = sizeof(long double);
    if (size % aligne == 0)
        return size;
    else
        return ((size + aligne - 1) / aligne) * aligne;
}

static void divise_block(struct blk *block, size_t size)
{
    if (block->size <= size + sizeof(struct blk))
        return;
    
    struct blk *newblock =
        (struct blk *)((char *)block + sizeof(struct blk) + size);
    newblock->size = block->size - size - sizeof(struct blk);
    newblock->freedom = 1;
    newblock->next = block->next;
    newblock->prev = block;
    newblock->page_start = block->page_start;
    newblock->page_size = 0;

    if (block->next != NULL)
        block->next->prev = newblock;
    block->next = newblock;
    block->size = size;
}

__attribute__((visibility("default"))) void *malloc(size_t size)
{
    if (size == 0)
        return NULL;

    size_t a_size = align(size);
    struct blk *current = head;

    while (current != NULL)
    {
        if (current->freedom == 1 && current->size >= a_size)
        {
            current->freedom = 0;
            divise_block(current, a_size);

            return (void *)(current + 1);
        }
        current = current->next;
    }

    struct blk *newblock = block_allocator(a_size);

    if (newblock == NULL)
        return NULL;

    newblock->freedom = 0;

    if (head == NULL)
        head = newblock;

    else
    {
        struct blk *last = head;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = newblock;
        newblock->prev = last;
    }
    return (void *)(newblock + 1);
}

static int are_contiguous(struct blk *b1, struct blk *b2)
{
    char *end_of_b1 = (char *)(b1 + 1) + b1->size;
    return (end_of_b1 == (char *)b2);
}

static void merge_free_block(void)
{
    struct blk *current = head;

    while (current != NULL && current->next != NULL)
    {
        if (current->freedom == 1 
            && current->next->freedom == 1 && current->page_start == current->next->page_start
            && are_contiguous(current, current->next))
        {
            current->size += sizeof(struct blk) + current->next->size;
            current->next = current->next->next;

            if (current->next != NULL)
            {
                current->next->prev = current;
            }
	    continue;
        }
        current = current->next;
    }
}



static void release_empty_pages(void)
{
    struct blk *current = head;

    while (current != NULL)
    {
        struct blk *next = current->next;

        if (current->page_start == current && current->freedom == 1)
        {
            int can_free = 1;
            if (current->next != NULL && current->next->page_start == current->page_start)
            {
                can_free = 0; 
            }

            if (can_free)
            {
                if (current->prev != NULL)
                    current->prev->next = current->next;
                else
                    head = current->next;

                if (current->next != NULL)
                    current->next->prev = current->prev;

                munmap(current, current->page_size);
            }
        }

        current = next;
    }
}



__attribute__((visibility("default"))) void free(void *ptr)
{
    if (ptr == NULL)
        return;

    struct blk *block = (struct blk *)ptr - 1;

    if (block == NULL)
        return;

    block->freedom = 1;
    merge_free_block();
    release_empty_pages();
}

__attribute__((visibility("default"))) void *calloc(size_t nmemb, size_t size)
{
    size_t sizetot = nmemb * size;
    if (nmemb != 0 && sizetot / nmemb != size)
        return NULL;
    void *ptr = malloc(sizetot);
    if (ptr == NULL)
        return NULL;
    char *iptr = ptr;
    for (size_t i = 0; i < sizetot; i++)
    {
        iptr[i] = 0;
    }
    return ptr;
}

__attribute__((visibility("default"))) void *realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return malloc(size);

    if (size == 0)
    {
        free(ptr);
        return NULL;
    }

    struct blk *info_block = (struct blk *)ptr - 1;

    if (info_block->size >= size)
        return ptr;

    void *new_ptr = malloc(size);

    if (new_ptr != NULL)
    {
        memcpy(new_ptr, ptr, info_block->size);

        free(ptr);
    }
    return new_ptr;
}
