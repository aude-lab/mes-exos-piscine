#pragma once

#include <memory>
#include <vector>

#include "queen.hh"

//! forward declaration
class Queen;
class Worker;
class Provider;
class Nurturer;

/*
 * Colony class:
 *  - Manages the queen and the collection of workers.
 */
class Colony
{
public:
    Colony() = default;

    // Add a copy of the given ant to this colony.
    void addAnt(const Ant& a);

    // Single simulation step (queen action + workers perform their tasks).
    void step();

    // Used to feed the queen.
    void feed_queen(Nurturer& nurturer);

    // Get the workers of the colony.
    const std::vector<std::shared_ptr<Worker>>& workers() const;

    // Display the current colony state.
    void display() const;

private:
    std::unique_ptr<Queen> queen_ = nullptr;
    std::vector<std::shared_ptr<Worker>> workers_;
};
