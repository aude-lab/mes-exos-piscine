#include <stdio.h>
#include <stddef.h>

void sieve(int n)
{
    if (n < 2)
        return;
    
    int primes[1001] = {0}; 
    
    for (int i = 2; i * i <= n; i++)
    {
        if (primes[i] == 0) 
        {
            for (int j = i * i; j <= n; j += i)
            {
                primes[j] = 1;
	    }
        }
    }
    
    for (int i = 2; i <= n; i++)
    {
        if (primes[i] == 0)
        {
            printf("%d\n", i);
        }
    }
}
