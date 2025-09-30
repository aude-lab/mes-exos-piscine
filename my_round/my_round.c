#include <stdio.h>

int my_round(float n)
{
    if (n > 0)
    {
        int ent = n;
        float dec = n - ent;

        if (dec >= 0.5)
            return ent + 1;
        return ent;
    }
    else
    {
        int enty = n;
        float decy = n - enty;
        if (decy == 0)
        {
            return enty;
        }
        else if (decy > -0.5)
        {
            return enty;
        }
        else
        {
            return enty - 1;
        }
    }
}
