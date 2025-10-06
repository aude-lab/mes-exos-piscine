#include "compute_price.h"

#include <math.h>

double compute_price(double quantity)
{
    return exp(quantity) + pow(quantity, sin(quantity));
}
