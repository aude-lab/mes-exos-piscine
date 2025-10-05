#include "fifo.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct fifo *fifo_init(void)
{
    struct fifo *fifo = malloc(sizeof(struct fifo));
    if (!fifo)
        return NULL;
    fifo->head = NULL;
    fifo->tail = NULL;
    fifo->size = 0;
    return fifo;
}

size_t fifo_size(struct fifo *fifo)
{
    return fifo->size;
}

void fifo_push(struct fifo *fifo, int elt)
{
    struct list *new = malloc(sizeof(struct list));
    if (!new)
        return;
    new->data = elt;
    new->next = NULL;
    if (!fifo->head)
    {
        fifo->head = new;
        fifo->tail = new;
    }
    else
    {
        fifo->tail->next = new;
        fifo->tail = new;
    }
    fifo->size++;
}

int fifo_head(struct fifo *fifo)
{
    return fifo->head->data;
}

void fifo_pop(struct fifo *fifo)
{
    if (!fifo->head)
        return;
    struct list *temp = fifo->head;
    fifo->head = fifo->head->next;
    if (!fifo->head)
        fifo->tail = NULL;
    free(temp);
    fifo->size--;
}

void fifo_clear(struct fifo *fifo)
{
    struct list *verif = fifo->head;
    while (verif)
    {
        struct list *next = verif->next;
        free(verif);
        verif = next;
    }
    fifo->head = NULL;
    fifo->tail = NULL;
    fifo->size = 0;
}

void fifo_destroy(struct fifo *fifo)
{
    fifo_clear(fifo);
    free(fifo);
}

void fifo_print(const struct fifo *fifo)
{
    struct list *cur = fifo->head;
    while (cur)
    {
        printf("%d\n", cur->data);
        cur = cur->next;
    }
}
