#ifndef FIFO_H
#define FIFO_H

#include <stddef.h>

struct list
{
    int data;
    struct list *next;
};

struct fifo
{
    struct list *head;
    struct list *tail;
    size_t size;
};

/*
** Create a new queue and initialize it.
** Returns `NULL` if an error occured.
*/
struct fifo *fifo_init(void);

/*
** Returns the number of elements in the queue.
*/
size_t fifo_size(struct fifo *fifo);

/*
** Add a new element to the queue.
*/
void fifo_push(struct fifo *fifo, int elt);

/*
** Returns the head element of the queue.
*/
int fifo_head(struct fifo *fifo);

/*
** Remove the head of the queue.
*/
void fifo_pop(struct fifo *fifo);

/*
** Remove all elements from the queue.
** `fifo` must be still usable.
*/
void fifo_clear(struct fifo *fifo);

/*
** Empty the queue and free `fifo`.
*/
void fifo_destroy(struct fifo *fifo);

/*
** Display the elements of `fifo` from head to tail.
*/
void fifo_print(const struct fifo *fifo);

#endif /* ! FIFO_H */
