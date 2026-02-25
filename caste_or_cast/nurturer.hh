#pragma once

#include "worker.hh"

/*
 * Nurturer sub-class
 */
class Nurturer : public Worker
{
public:
    using Worker::Worker;

    //! Perform all Nurturer tasks when adult.
    void work() override;

    //! Communicate with another ant.
    bool communicate(Ant* other) override;
};
