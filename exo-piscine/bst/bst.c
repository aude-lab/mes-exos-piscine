#include "bst.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct bst_node *create_node(int value)
{
    struct bst_node *new = malloc(sizeof(struct bst_node));
    if (!new)
        return NULL;
    new->data = value;
    new->left = NULL;
    new->right = NULL;
    return new;
}

struct bst_node *add_node(struct bst_node *tree, int value)
{
    if (!tree)
        return create_node(value);
    if (value == tree->data)
        return tree;
    if (value < tree->data)
        tree->left = add_node(tree->left, value);
    else
        tree->right = add_node(tree->right, value);
    return tree;
}

struct bst_node *delete_node(struct bst_node *tree, int value)
{
    if (!tree)
        return NULL;
    if (value < tree->data)
        tree->left = delete_node(tree->left, value);
    else if (value > tree->data)
        tree->right = delete_node(tree->right, value);
    else
    {
        if (!tree->left)
        {
            struct bst_node *transit = tree->right;
            free(tree);
            return transit;
        }
        else if (!tree->right)
        {
            struct bst_node *transit = tree->left;
            free(tree);
            return transit;
        }
        else
        {
            struct bst_node *transit = tree->left;
            while (transit && transit->right)
                transit = transit->right;

            tree->data = transit->data;
            tree->left = delete_node(tree->left, transit->data);
        }
    }
    return tree;
}

const struct bst_node *find(const struct bst_node *tree, int value)
{
    if (!tree)
        return NULL;
    if (value == tree->data)
        return tree;
    if (value < tree->data)
        return find(tree->left, value);
    else
        return find(tree->right, value);
}

void free_bst(struct bst_node *tree)
{
    if (!tree)
        return;
    free_bst(tree->left);
    free_bst(tree->right);
    free_bst(tree);
}
