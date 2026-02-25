#pragma once

#include "ant.hh"

/*
 * Abstract Worker class. Represents ants that perform tasks.
 */
class Worker : public Ant
{
public:
    using Ant::Ant;
    virtual ~Worker() = default;
    //! Do the work for the worker. Implemented by concrete subclasses.
    virtual void work() = 0;
};
