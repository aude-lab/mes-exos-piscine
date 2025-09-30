#ifndef PAIRS_H
#define PAIRS_H

#include <stddef.h>

struct pair
{
    int x;
    int y;
};

struct pair three_pairs_sum(const struct pair pair_1, const struct pair pair_2,
                            const struct pair pair_3);

struct pair pairs_sum(const struct pair pairs[], size_t size);

#endif
