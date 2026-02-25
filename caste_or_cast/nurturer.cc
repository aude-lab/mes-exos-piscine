#include "nurturer.hh"

#include <iostream>

#include "colony.hh"
#include "provider.hh"

void Nurturer::work()
{
    // FIXME: implement the Nurturer::work() logic

    if (stage_ != Stage::ADULT)
        return;
    std::cout << "Nurturer " << id_ << " works\n";

    auto colony = colony_.lock();
    if (!colony)
        return;

    if (food_ <= 0)
    {
        for (auto& worker : colony->workers())
        {
            communicate(worker.get()); 
        }
        return;
    }

    for (auto& worker : colony->workers())
    {
        if (worker->get_stage() != Stage::ADULT && worker->get_food() == 0)
        {
            give_food(*worker);
            return;
        }
    }
    colony->feed_queen(*this);
}

bool Nurturer::communicate(Ant* other)
{
    if (!Ant::communicate(other))
        return false;

    Provider* p = dynamic_cast<Provider*>(other);

    if (p)
        return p->give_all_food(*this);

    return false;
}
