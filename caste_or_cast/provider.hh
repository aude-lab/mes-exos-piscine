#pragma once

#include "worker.hh"

class Nurturer;

/*
 * Provider sub-class
 */
class Provider : public Worker
{
public:
    using Worker::Worker;

    //! Perform all Provider tasks when adult.
    void work() override;

    //! Give all the food to a nurturer, returns true if succeed
    bool give_all_food(Nurturer& n);
};
