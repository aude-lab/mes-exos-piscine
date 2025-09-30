#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void sieve(int n)
{
    if (n <= 2) 
        return;
    bool *is_not_prime = calloc(n, sizeof(bool));
    if (!is_not_prime) {
        return;  
    }
    
    int count = 0;
    for (int i = 2; i < n; i++) {
            if (!is_not_prime[i]) {
            count++;
            for (int j = i * 2; j < n; j += i) {
                is_not_prime[j] = true;
            }
        }
    }
    
    printf("%d\n", count);
    free(is_not_prime);
}


