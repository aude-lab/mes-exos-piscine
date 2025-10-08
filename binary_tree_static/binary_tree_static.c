#include "binary_tree_static.h"

#include <stddef.h>
#include <stdio.h>

size_t size(const struct binary_tree *tree)
{
    if (!tree)
        return 0;
    size_t tree_siz = 0;
    for (size_t i = 0; i <= tree->max_index; i++)
    {
        if (tree->data[i])
            tree_siz++;
    }
    return tree_siz;
}

int height(const struct binary_tree *tree)
{
    if (!tree || !tree->data[0])
        return -1;
    int heig = -1;
    for (size_t i = 0; i <= tree->max_index; i++)
    {
        if (tree->data[i])
        {
            int dep = 0;
            size_t cur = i;
            while (cur > 0)
            {
                dep++;
                cur = (cur - 1) / 2;
            }
            if (dep > heig)
                heig = dep;
        }
    }
    return heig;
}

void aux_print_prefix(const struct binary_tree *tree, size_t index)
{
    if (index >= CAPACITY || !tree->data[index])
        return;
    printf("%d ", tree->data[index]->val);

    aux_print_prefix(tree, 2 * index + 1);
    aux_print_prefix(tree, 2 * index + 2);
}

void dfs_print_prefix(const struct binary_tree *tree)
{
    if (!tree || !tree->data[0])
        return;
    aux_print_prefix(tree, 0);
}

void bfs_print(const struct binary_tree *tree)
{
    if (!tree || !tree->data[0])
        return;
    int level_one = 1;
    for (size_t i = 0; i <= tree->max_index; i++)
    {
        if (tree->data[i])
        {
            size_t lsize = 1;
            size_t cur_begin = 0;

            while (cur_begin + lsize <= i)
            {
                cur_begin += lsize;
                lsize *= 2;
            }
            if (i == cur_begin)
            {
                if (!level_one)
                    printf("| ");
            }
            if (level_one)
                level_one = 0;

            printf("%d", tree->data[i]->val);
        }
    }
}

int is_complete(const struct binary_tree *tree)
{
    if (!tree || !tree->data[0])
        return 1;
    int verif_null = 0;
    for (size_t i = 0; i <= tree->max_index; i++)
    {
        if (tree->data[i] == NULL)
            verif_null = 1;
        else
        {
            if (verif_null == 1)
                return 0;
        }
    }
    return 1;
}

int is_perfect(const struct binary_tree *tree)
{
    if (!tree || !tree->data[0])
        return 1;
    int heig = height(tree);
    size_t good_nu_nods = (1 << (heig + 1)) - 1;
    for (size_t i = 0; i < good_nu_nods && i < CAPACITY; i++)
    {
        if (tree->data[i] == NULL)
            return 0;
    }

    if (tree->max_index >= good_nu_nods)
    {
        for (size_t i = good_nu_nods; i <= tree->max_index; i++)
        {
            if (tree->data[i] != NULL)
                return 0;
        }
    }
    return 1;
}
