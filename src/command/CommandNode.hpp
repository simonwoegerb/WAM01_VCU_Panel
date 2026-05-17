#pragma once
#include <vector>
#include <memory>
#include <string>
#include <functional>

#include "CommandContext.hpp"
#include "Value.hpp"

struct CommandNode
{
    enum class Type { Root, Literal, Argument };

    std::string name;
    Type type;

    std::vector<std::unique_ptr<CommandNode>> children;

    struct ArgSpec
    {
        std::string name;
        std::function<bool(const std::string&, Value&)> parse;
    } arg;

    std::function<void(CommandContext&)> executor;

    CommandNode(std::string n, Type t)
        : name(std::move(n)), type(t) {}

    CommandNode* add(std::unique_ptr<CommandNode> child)
    {
        children.push_back(std::move(child));
        return children.back().get();
    }
};
