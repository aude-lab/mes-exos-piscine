#include "traffic_lights.h"

#include <stdio.h>

void init(unsigned char *lights)
{
    *lights = *lights & ~(*lights);
}

void turn_on(unsigned char *lights, unsigned char light_num)
{
    unsigned char one = *lights | (1 << (light_num - 1));
    if (light_num <= 4 && light_num >=1
    	*lights = one;
}

void turn_off(unsigned char *lights, unsigned char light_num)
{
    if (light_num <= 4 && light_num >=1
        *lights = *lights & ~(1 << (light_num - 1));
}

void next_step(unsigned char *lights)
{
    unsigned char one = (*lights & (1 << 3)) >> 3;
    *lights = (*lights << 1) | bit;
    *lights &= 0x0F;
}

void reverse(unsigned char *lights)
{
    *lights ^= 0x0F;
}

void swap(unsigned char *lights_1, unsigned char *lights_2)
{
    if (lights_1 != lights_2)
    {
        *lights_1 ^= *lights_2;
        *lights_2 ^= *lights_1;
        *lights_1 ^= *lights_2;
    }
}
