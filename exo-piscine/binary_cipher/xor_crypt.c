#include <stddef.h>
#include <stdio.h>

#include "binary_cipher.h"

void my_xor_crypt(void *data, size_t data_len, const void *key, size_t key_len)
{
    unsigned char *data_b = data;
    const unsigned char *key_b = key;
    for (size_t i = 0; i < data_len; i++)
    {
        data_b[i] ^= key_b[i % key_len];
    }
}
