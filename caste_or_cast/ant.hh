#pragma once

#include <iostream>
#include <memory>

//! Forward declaration
class Colony;

/*
 * Enum representing the main stages of an ant's development.
 */
enum class Stage
{
    EGG = 0,
    LARVA,
    ADULT
};

/*
 * Base class for every ant.
 */
class Ant
{
public:
    //! Construct an Ant belonging to the given colony at the given stage.
    Ant(std::shared_ptr<Colony> c, Stage s);

    //! Default virtual destructor to allow polymorphic deletion.
    virtual ~Ant() = default;

    //! Every ant consume food to grow.
    void live();

    //! Return the development stage of the ant.
    Stage get_stage() const;

    //! Return the current food of the ant.
    int get_food() const;

    //! Give one food to this ant.
    void give_food(Ant& other);

    /*!
     * Communicate with another ant.
     */
    virtual bool communicate(Ant* other);

    // operator<< support
    friend std::ostream& operator<<(std::ostream& os, const Ant& ant);

protected:
    //! Colony that this ant belongs to.
    std::weak_ptr<Colony> colony_;

    //! current development stage.
    Stage stage_;

    //! current food of the ant.
    int food_;

    //! Unique ant ID
    int id_;

    //! Static counter shared by all ants
    static int counter_;
};
