#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

unsigned int min3(unsigned int a, unsigned int b, unsigned int c)
{
    unsigned int min = a;
    if (b < min)
        min = b;
    if (c < min)
        min = c;
    return min;
}

unsigned int dlist_levenshtein(const struct dlist *list1,
                               const struct dlist *list2)
{
    if (!list1 || !list2)
        return 0;

    size_t len1 = list1->size;
    size_t len2 = list2->size;

    if (len1 == 0)
        return len2;
    if (len2 == 0)
        return len1;

    unsigned int **matrix = malloc((len1 + 1) * sizeof(unsigned int *));
    for (size_t i = 0; i <= len1; i++)
    {
        matrix[i] = malloc((len2 + 1) * sizeof(unsigned int));
    }

    for (size_t i = 0; i <= len1; i++)
        matrix[i][0] = i;
    for (size_t j = 0; j <= len2; j++)
        matrix[0][j] = j;

    struct dlist_item *item1 = list1->head;
    for (size_t i = 1; i <= len1; i++)
    {
        struct dlist_item *item2 = list2->head;
        for (size_t j = 1; j <= len2; j++)
        {
            int cost = (item1->data == item2->data) ? 0 : 1;

            matrix[i][j] = min3(matrix[i - 1][j] + 1, matrix[i][j - 1] + 1,
                                matrix[i - 1][j - 1] + cost);

            item2 = item2->next;
        }
        item1 = item1->next;
    }

    unsigned int result = matrix[len1][len2];

    for (size_t i = 0; i <= len1; i++)
        free(matrix[i]);
    free(matrix);

    return result;
}
