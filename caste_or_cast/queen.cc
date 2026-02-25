#include "queen.hh"

#include <iostream>

#include "colony.hh"
#include "nurturer.hh"
#include "provider.hh"

void Queen::layEgg()
{
    // FIXME: implement the Queen::layEgg() logic
    if (stage_ != Stage::ADULT || food_ < 5)
        return;

    std::cout << "Queen " << id_ << " lays an egg\n";
    bool is_nurturer;
    if (food_ % 2 == 0)
        is_nurturer = true;
    else
        is_nurturer = false;

    food_ -= 5;

    auto colony = colony_.lock();
    if (!colony)
        return;

    if (is_nurturer)
        colony->addAnt(Nurturer{ colony_.lock(), Stage::EGG });
    else
        colony->addAnt(Provider{ colony_.lock(), Stage::EGG });
}
