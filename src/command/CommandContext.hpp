#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include "Value.hpp"

struct CommandContext
{
    std::unordered_map<std::string, Value> args;

    std::function<void(const std::string&)> log;

    void Log(const std::string& msg)
    {
        if (log)
            log(msg);
    }
};
