#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct vector *vector_init(size_t n)
{
	struct vector *new = malloc(sizeof(struct vector));
	if (new == NULL)
        	return NULL;
	new->data = malloc(sizeof(int) * n);
	if (new->data == NULL)
	{
		free(new);
		return NULL;
	}
	new->size = 0;
	new->capacity = n;
	return v;
}


void vector_destroy(struct vector *v)
{
    if (v == NULL)
        return;

    free(v->data);
    free(v);
}


struct vector *vector_resize(struct vector *v, size_t n)
{
    if (v == NULL)
        return NULL;

    if (n == v->capacity)
        return v;

    int *new_data = realloc(v->data, n * sizeof(int));
    if (new_data == NULL)
        return NULL;

    v->data = new_data;
    v->capacity = n;

    if (v->size > n)
        v->size = n;

    return v;
}



struct vector *vector_append(struct vector *v, int elt)
{
    if (v == NULL)
        return NULL;

    if (v->size >= v->capacity) {
        v = vector_resize(v, v->capacity * 2);
        if (v == NULL)
            return NULL;
    }

    v->data[v->size] = elt;
    v->size++;
    return v;
}


void vector_print(const struct vector *v)
{
    if (v == NULL || v->size == 0) {
        printf("\n");
        return;
    }

    for (size_t i = 0; i < v->size; i++) {
        printf("%d", v->data[i]);
        if (i < v->size - 1)
            printf(",");
    }
    printf("\n");
}


struct vector *vector_reset(struct vector *v, size_t n)
{
    if (v == NULL)
        return NULL;

    v->size = 0;
    return vector_resize(v, n);
}


struct vector *vector_insert(struct vector *v, size_t i, int elt)
{
    if (v == NULL || i > v->size)
        return NULL;
    if (i == v->size)
        return vector_append(v, elt);

    if (v->size >= v->capacity) {
        v = vector_resize(v, v->capacity * 2);
        if (v == NULL)
            return NULL;
    }
    for (size_t j = v->size; j > i; j--) {
        v->data[j] = v->data[j - 1];
    }

    v->data[i] = elt;
    v->size++;
    return v;
}

struct vector *vector_remove(struct vector *v, size_t i)
{
    if (v == NULL || i >= v->size)
        return NULL;
    
    for (size_t j = i; j < v->size - 1; j++) {
        v->data[j] = v->data[j + 1];
    }
    
    v->size--;
    
    if (v->size < v->capacity / 2) {
        return vector_resize(v, v->capacity / 2);
    }
    
    return v;
}
