#include "my_strcasecmp.h"

int my_strcasecmp(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        char c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
        char c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;
        if (c1 != c2) {
            return c1 - c2;
        }
        s1++;
        s2++;
    }
    // À ce stade, une des chaînes est terminée
    char end1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
    char end2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;
    return end1 - end2;
}
