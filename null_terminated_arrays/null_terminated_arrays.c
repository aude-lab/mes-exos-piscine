#include "null_terminated_arrays.h"

static void reverse_array(const char **array)
{
    if (!array || !array[0])
        return;
    
    int len = 0;
    while (array[len] != NULL)
    {
        len++;
    }
    
    for (int i = 0; i < len / 2; i++)
    {
        const char *temp = array[i];
        array[i] = array[len - 1 - i];
        array[len - 1 - i] = temp;
    }
}

static int array_length(const char **array)
{
    int len = 0;
    if (array)
    {
        while (array[len] != NULL)
        {
            len++;
        }
    }
    return len;
}

void reverse_matrix(const char ***matrix)
{
    if (!matrix || !matrix[0])
        return;
    int i = 0;
    while (matrix[i] != NULL)
    {
        reverse_array(matrix[i]);
        i++;
    }
    
    int matrix_len = 0;
    while (matrix[matrix_len] != NULL)
    {
        matrix_len++;
    }
    
    for (int j = 0; j < matrix_len / 2; j++)
    {
        const char **temp = matrix[j];
        matrix[j] = matrix[matrix_len - 1 - j];
        matrix[matrix_len - 1 - j] = temp;
    }
}
