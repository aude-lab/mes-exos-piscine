#include "builder.hh"
#include "components.hh"
#include "car.hh"

Car Builder::get_car() const
{
    Car car;

    for (auto& wheel : car.wheels_)
        wheel = get_wheel();

    car.engine_ = get_engine();
    car.body_   = get_body();

    return car;
}

std::unique_ptr<Wheel> JeepBuilder::get_wheel() const
{
    return std::make_unique<Wheel>(22);
}

std::unique_ptr<Engine> JeepBuilder::get_engine() const
{
    return std::make_unique<Engine>(400);
}

std::unique_ptr<Body> JeepBuilder::get_body() const
{
    return std::make_unique<Body>("SUV");
}

std::unique_ptr<Wheel> NissanBuilder::get_wheel() const
{
    return std::make_unique<Wheel>(16);
}

std::unique_ptr<Engine> NissanBuilder::get_engine() const
{
    return std::make_unique<Engine>(85);
}

std::unique_ptr<Body> NissanBuilder::get_body() const
{
    return std::make_unique<Body>("hatchback");
}
