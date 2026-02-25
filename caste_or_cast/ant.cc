#include "ant.hh"

#include "colony.hh"

int Ant::counter_ = 0;

Ant::Ant(std::shared_ptr<Colony> c, Stage s)
    : colony_(c)
    , stage_(s)
    , food_(0)
    , id_(++counter_)
{}

void Ant::live()
{
    if (food_ >= 1 && stage_ != Stage::ADULT)
    {
        stage_ = static_cast<Stage>(static_cast<int>(stage_) + 1);
        food_--;
    }
}

Stage Ant::get_stage() const
{
    return stage_;
}

int Ant::get_food() const
{
    return food_;
}

void Ant::give_food(Ant& other)
{
    other.food_++;
    food_--;
}

bool Ant::communicate(Ant* other)
{
    // FIXME: a Ant must communicate only if they live in the same colony
    auto my_colony = colony_.lock();
    auto other_colony = other->colony_.lock();
    if (!my_colony)
        return false;
    if (!other_colony)
        return false;
    if (my_colony != other_colony)
        return false;
    return true;

}

std::ostream& operator<<(std::ostream& os, const Ant& ant)
{
    os << "Ant #" << ant.id_ << " | Stage: " << static_cast<int>(ant.stage_)
       << " | Food: " << ant.food_;
    return os;
}
