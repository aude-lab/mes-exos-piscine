#include <stddef.h>
#include <stdio.h>

#include "binary_cipher.h"

void my_rol_crypt(void *data, size_t data_len, const void *key, size_t key_len)
{
    if (key_len == 0 || data_len == 0)
	    return;
    unsigned char *data_b = data;
    const unsigned char *key_b = key;
    for (size_t i = 0; i < data_len; i++)
    {
        data_b[i] += key_b[i % key_len];
    }
}	
