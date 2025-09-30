#include "levenshtein.h"
#include <stdlib.h>

size_t levenshtein(const char *s1, const char *s2)
{
    if (!s1 || !s2) return 0;
    
    size_t len1 = 0;
    size_t len2 = 0;
    while (s1[len1]) 
	    len1++;
    while (s2[len2]) 
	    len2++;
    
    if (len1 == 0) 
	    return len2;
    if (len2 == 0) 
	    return len1;
    
    size_t *prev = malloc((len2 + 1) * sizeof(size_t));
    size_t *curr = malloc((len2 + 1) * sizeof(size_t));
    
    for (size_t j = 0; j <= len2; j++)
        prev[j] = j;
    
    for (size_t i = 1; i <= len1; i++)
    {
        curr[0] = i;
        
        for (size_t j = 1; j <= len2; j++)
        {
            size_t cost = (s1[i-1] == s2[j-1]) ? 0 : 1;
            
            size_t deletion = prev[j] + 1;
            size_t insertion = curr[j-1] + 1;
            size_t substitution = prev[j-1] + cost;
            
            size_t min = deletion;
            if (insertion < min) min = insertion;
            if (substitution < min) min = substitution;
            
            curr[j] = min;
        }
        
        size_t *temp = prev;
        prev = curr;
        curr = temp;
    }
    
    size_t result = prev[len2];
    free(prev);
    free(curr);
    
    return result;
}
