#include "levenshtein.h"

#include <stdio.h>
#include <stdlib.h>

size_t my_strlen(const char *s)
{
    size_t len = 0;
    while (s[len] != '\0')
        len++;
    return len;
}

size_t min3(size_t a, size_t b, size_t c)
{
    size_t m = a;
    if (b < m)
        m = b;
    if (c < m)
        m = c;
    return m;
}

void init_previous(size_t *prev, size_t len2)
{
    for (size_t j = 0; j <= len2; j++)
        prev[j] = j;
}

size_t cal_min_distance(const char *s1, const char *s2, size_t len1,
                        size_t len2)
{
    size_t *prev = malloc((len2 + 1) * sizeof(size_t));
    size_t *curr = malloc((len2 + 1) * sizeof(size_t));
    if (!prev || !curr)
        return 0;

    init_previous(prev, len2);

    for (size_t i = 1; i <= len1; i++)
    {
        curr[0] = i;
        for (size_t j = 1; j <= len2; j++)
        {
            size_t cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            curr[j] = min3(prev[j] + 1, curr[j - 1] + 1, prev[j - 1] + cost);
        }
        size_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    size_t result = prev[len2];
    free(prev);
    free(curr);
    return result;
}

size_t levenshtein(const char *s1, const char *s2)
{
    if (!s1 || !s2)
        return 0;
    size_t len1 = my_strlen(s1), len2 = my_strlen(s2);
    if (len1 == 0)
        return len2;
    if (len2 == 0)
        return len1;
    return cal_min_distance(s1, s2, len1, len2);
}
