#include <stdio.h>
#include <stdlib.h>

#include "evalexpr.h"

struct stack *stack_create(void)
{
    struct stack *s = malloc(sizeof(struct stack));
    if (!s)
        return NULL;

    s->capacity = 10;
    s->size = 0;
    s->data = malloc(s->capacity * sizeof(int));
    if (!s->data)
    {
        free(s);
        return NULL;
    }
    return s;
}

void stack_push(struct stack *s, int value)
{
    if (s->size >= s->capacity)
    {
        s->capacity *= 2;
        s->data = realloc(s->data, s->capacity * sizeof(int));
    }
    s->data[s->size++] = value;
}

int stack_is_empty(struct stack *s)
{
    return s->size == 0;
}


int stack_pop(struct stack *s)
{
    if (s->size == 0)
        return 0;
    return s->data[--s->size];
}

int stack_peek(struct stack *s)
{
    if (s->size == 0)
        return 0;
    return s->data[s->size - 1];
}

void stack_free(struct stack *s)
{
    if (s)
    {
        free(s->data);
        free(s);
    }
}
