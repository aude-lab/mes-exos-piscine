#include "bst_static.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct bst *init(size_t capacity)
{
    struct bst *new = malloc(sizeof(struct bst));
    if (!new)
        return NULL;
    new->capacity = capacity;
    new->size = 0;
    new->data = calloc(capacity, sizeof(struct value *));
    if (!new->data)
    {
        free(new);
        return NULL;
    }
    return new;
}

void add(struct bst *tree, int value)
{
    if (!tree)
        return;

    size_t i = 0;
    while (i < tree->capacity && tree->data[i] != NULL)
    {
        if (value == tree->data[i]->val)
            return;
        if (value < tree->data[i]->val)
            i = 2 * i + 1;
        else
            i = 2 * i + 2;
    }

    if (i >= tree->capacity)
    {
        size_t nouv_capacity = i + 1;
        struct value **new_data =
            realloc(tree->data, nouv_capacity * sizeof(struct value *));
        if (!new_data)
            return;

        for (size_t j = tree->capacity; j < nouv_capacity; j++)
            new_data[j] = NULL;
        tree->data = new_data;
        tree->capacity = nouv_capacity;
    }
    struct value *val_new = malloc(sizeof(struct value));
    if (!val_new)
        return;
    val_new->val = value;
    tree->data[i] = val_new;
    tree->size++;
}

int search(struct bst *tree, int value)
{
    if (!tree)
        return -1;
    size_t index = 0;
    while (index < tree->capacity && tree->data[index] != NULL)
    {
        if (value == tree->data[index]->val)
            return index;
        if (value < tree->data[index]->val)
            index = 2 * index + 1;
        else
            index = 2 * index + 2;
    }
    return -1;
}

void bst_free(struct bst *tree)
{
    if (!tree)
        return;
    if (tree->data)
    {
        for (size_t i = 0; i < tree->capacity; i++)
        {
            if (tree->data[i])
                free(tree->data[i]);
        }
        free(tree->data);
    }
    free(tree);
}
