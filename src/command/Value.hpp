#pragma once
#include <variant>
#include <string>

struct Value
{
    std::variant<int, float, bool, std::string> data;

    template<typename T>
    T as() const
    {
        return std::get<T>(data);
    }
};
