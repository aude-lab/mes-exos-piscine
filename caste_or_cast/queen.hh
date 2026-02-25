#pragma once

#include "ant.hh"

/*
 * Queen class
 */
class Queen : public Ant
{
public:
    using Ant::Ant;

    //! Lay a new egg
    void layEgg();
};
