#include "colony.hh"

#include <iostream>

#include "nurturer.hh"
#include "provider.hh"
#include "queen.hh"

void Colony::addAnt(const Ant& a)
{
    // FIXME: implement detection and copying using RTTI.
    if (const Queen* q = dynamic_cast<const Queen*>(&a))
    {
        if (queen_)
            throw std::runtime_error("Colony has a queen");
        if (q->get_stage() != Stage::ADULT)
            throw std::runtime_error("Attempt to add non-adult Queen");
        queen_ = std::make_unique<Queen>(*q);
    }
    else if (const Provider* p = dynamic_cast<const Provider*>(&a))
    {
        workers_.push_back(std::make_shared<Provider>(*p));
    }
    else if (const Nurturer* n = dynamic_cast<const Nurturer*>(&a))
    {
        workers_.push_back(std::make_shared<Nurturer>(*n));
    }
    else
    {
        throw std::runtime_error("unknown Ant subtype");
    }

}

void Colony::step()
{
    // FIXME: to implement
    if (queen_)
    {
        queen_->live();
        queen_->layEgg();
    }
    for (auto& worker : workers_)
    {
        worker->live();
        worker->work();
    }
}

void Colony::feed_queen(Nurturer& nurturer)
{
    if (queen_)
    {
        nurturer.give_food(*queen_);
    }
}

const std::vector<std::shared_ptr<Worker>>& Colony::workers() const
{
    return workers_;
}

void Colony::display() const
{
    std::cout << "===== Colony state =====" << std::endl;

    if (queen_)
        std::cout << "Queen: " << *queen_ << std::endl;
    else
        std::cout << "Queen: (none)\n";

    std::cout << "Workers: " << workers_.size() << std::endl;
    for (const auto& w : workers_)
        std::cout << " - " << *w << std::endl;
}
