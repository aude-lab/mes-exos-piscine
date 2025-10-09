#include "insertion_sort.h"
#include <stdio.h>
#include <stddef.h>

typedef int (*f_cmp)(const void *, const void *);

void insertion_sort(void **array, f_cmp comp)
{
    if (!array || !comp)
        return;

    size_t size = 0;
    while (array[size])
    {
        size++;
    }

    for (size_t i = 1; i < size; i++)
    {
        void *key = array[i];
        size_t j = i;

        while (j > 0 && comp(array[j - 1], key) > 0)
        {
            array[j] = array[j - 1];
            j--;
        }
        array[j] = key;
    }
}
