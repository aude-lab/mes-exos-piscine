#include "provider.hh"

#include <iostream>

#include "nurturer.hh"

void Provider::work()
{
    // FIXME: implement the Provider::work() logic
    if (stage_ != Stage::ADULT)
        return;
    std::cout << "Provider " << id_ << " works\n";
    food_++;
}

bool Provider::give_all_food(Nurturer& n)
{
    // FIXME: implement the Provider::give_all_food() logic
    if (food_ <= 0)
        return false;
    std::cout << "Provider " << id_ << " gives all food\n";
    while (food_ > 0)
        give_food(n);

    return true;

}
