#include "pairs.h"

struct pair three_pairs_sum(const struct pair pair_1, const struct pair pair_2,
                            const struct pair pair_3)
{
    struct pair result;
    
    result.x = pair_1.x + pair_2.x + pair_3.x;
    result.y = pair_1.y + pair_2.y + pair_3.y;
    
    return result;
}

struct pair pairs_sum(const struct pair pairs[], size_t size)
{
    struct pair result = {0, 0}; 
    if (size == 0)
        return result;
    
    for (size_t i = 0; i < size; i++)
    {
        result.x += pairs[i].x;
        result.y += pairs[i].y;
    }
    
    return result;
}
