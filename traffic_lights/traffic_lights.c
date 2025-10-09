#include "traffic_lights.h"

void init(unsigned char *lights)
{
    *lights = 0;
}

void turn_on(unsigned char *lights, unsigned char light_num)
{
    unsigned char one = *lights & ~*lights;
    one = one | ~one;
    *lights = *lights | (one << light_num);
}

void turn_off(unsigned char *lights, unsigned char light_num)
{
    unsigned char one = *lights & ~*lights;
    one = one | ~one;
    *lights = *lights & ~(one << light_num);
}

void next_step(unsigned char *lights)
{
    unsigned char one = *lights & ~*lights;
    one = one | ~one;
    unsigned char last_bit = *lights & (one << (one + one + one));
    *lights = (*lights >> one) | (last_bit ? one : *lights & ~*lights);
}

void reverse(unsigned char *lights)
{
    *lights ^= 0x0F;
}

void swap(unsigned char *lights_1, unsigned char *lights_2)
{
    *lights_1 ^= *lights_2;
    *lights_2 ^= *lights_1;
    *lights_1 ^= *lights_2;
}
