#pragma once

#include <concepts>
#include <iostream>
#include <vector>

template <typename T, typename U>
concept Mul = requires(T t, U u)
{
    { t * u } -> std::convertible_to<T>;
};

template <typename T>
concept Summable = requires(T a, T b)
{
    { a + b } -> std::convertible_to<T>;
};

template <typename T>
class MyVector
{
public:
    MyVector() = default;

    MyVector(const std::vector<T>& v)
        : vec_{ v }
    {}

    template <typename U>
        requires Mul<T, U>
        MyVector<T>
    operator*(const U& scalar)
    {
        std::vector<T> result{ vec_ };
        for (auto& elem : result)
            elem *= scalar;
        return MyVector<T>{ result };
    }

    template <typename U>
        requires Mul<T, U>
        MyVector<T>
    & operator*=(const U& scalar)
    {
        for (auto& elem : vec_)
            elem *= scalar;
        return *this;
    }

    T reduce(T init) const requires Summable<T>
    {
        for (const auto& elem : vec_)
            init = init + elem;
        return init;
    }

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const MyVector<U>& vec);

private:
    std::vector<T> vec_;
};

#include "vector.hxx"
