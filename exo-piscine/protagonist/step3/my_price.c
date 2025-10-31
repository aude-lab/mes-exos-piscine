#include <stdio.h>

#include "compute_price.h"
#include "compute_quantity.h"

int main(void)
{
    double quantity = compute_quantity(2);
    double price = compute_price(quantity);
    printf("Price is $%.5f\n", price);
    return 0;
}
