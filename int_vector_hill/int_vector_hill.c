#include "int_vector_hill.h"

#include <stdio.h>

int int_vector_hill(struct int_vector vec)
{
    if (vec.size == 0)
        return -1;

    for (size_t i = 0; i < vec.size; i++)
    {
        if (vec.data[i] < 0)
            return -1;
    }
    size_t i = 0;
    while (i < vec.size - 1 && vec.data[i] <= vec.data[i + 1])
        i++;

    int hill_sommet = i;

    for (size_t j = hill_sommet; j < vec.size - 1; j++)
        if (vec.data[j] < vec.data[j + 1])
            return -1;

    return hill_sommet;
}
