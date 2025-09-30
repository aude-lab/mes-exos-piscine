#include "insertion_sort.h"

void insertion_sort(void **array, f_cmp comp)
{
    if (!array || !comp)
        return;

    int size = 0;
    while (array[size] != NULL)
    {
        size++;
    }

    for (int i = 1; i < size; i++)
    {
        void *key = array[i];
        int j = i - 1;

        while (j >= 0 && comp(array[j], key) > 0)
        {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
}
