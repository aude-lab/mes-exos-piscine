#include "evalexpr.h"
#include <stdlib.h>
#include <stdio.h>

Stack *stack_create(void)
{
    Stack *s = malloc(sizeof(Stack));
    if (!s)
        return NULL;
    
    s->capacity = 10;
    s->size = 0;
    s->data = malloc(s->capacity * sizeof(int));
    if (!s->data) {
        free(s);
        return NULL;
    }
    return s;
}

void stack_push(Stack *s, int value)
{
    if (s->size >= s->capacity) {
        s->capacity *= 2;
        s->data = realloc(s->data, s->capacity * sizeof(int));
    }
    s->data[s->size++] = value;
}

int stack_pop(Stack *s)
{
    if (s->size == 0)
        return 0;
    return s->data[--s->size];
}

int stack_peek(const Stack *s)
{
    if (s->size == 0)
        return 0;
    return s->data[s->size - 1];
}

int stack_is_empty(const Stack *s)
{
    return s->size == 0;
}

void stack_free(Stack *s)
{
    if (s) {
        free(s->data);
        free(s);
    }
}
