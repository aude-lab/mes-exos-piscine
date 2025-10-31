#include "binary_tree.h"
#include <stdio.h>

int size(const struct binary_tree *tree)
{
	if (tree == NULL)
		return 0;
	return 1 + size(tree->left) + size(tree->right);
}


int height(const struct binary_tree *tree)
{
	if (tree == NULL)
		return -1;
	int left_height = height(tree->left);
	int right_height = height(tree->right);
	int max = 0;
	if (left_height >right_height)
		max = left_height;
	
	else
		max = right_height;
	return 1 + max;
}


void dfs_print_prefix(const struct binary_tree *tree)
{
	if (tree == NULL)
        return;

    printf("%d ", tree->data);
    dfs_print_prefix(tree->left);
    dfs_print_prefix(tree->right);
}


void dfs_print_infix(const struct binary_tree *tree)
{
    if (tree == NULL)
        return;

    dfs_print_infix(tree->left);
    printf("%d ", tree->data);
    dfs_print_infix(tree->right);
}

void dfs_print_postfix(const struct binary_tree *tree)
{
    if (tree == NULL)
        return;

    dfs_print_postfix(tree->left);
    dfs_print_postfix(tree->right);
    printf("%d ", tree->data);
}



int is_perfect(const struct binary_tree *tree)
{
    if (tree == NULL)
        return 1;

    int left_height = height(tree->left);
    int right_height = height(tree->right);

    if (left_height != right_height)
        return 0;

    return is_perfect(tree->left) && is_perfect(tree->right);
}



int is_degenerate(const struct binary_tree *tree)
{
    if (tree == NULL)
        return 1;

    if (tree->left != NULL && tree->right != NULL)
        return 0;

    return is_degenerate(tree->left) && is_degenerate(tree->right);
}



int is_full(const struct binary_tree *tree)
{
    if (tree == NULL)
        return 1;

    if ((tree->left == NULL) != (tree->right == NULL))
        return 0;

    return is_full(tree->left) && is_full(tree->right);
}
