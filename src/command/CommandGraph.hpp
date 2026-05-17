#pragma once
#include <string>
#include <vector>
#include "CommandNode.hpp"

class CommandGraph
{
public:
    CommandNode root{"root", CommandNode::Type::Root};

bool Execute(const std::string& input,
             std::function<void(const std::string&)> logger,
             std::string& err);

    std::vector<std::string> Complete(const std::string& input);

private:
    std::vector<std::string> Tokenize(const std::string& s);

    bool Walk(CommandNode* node,
              const std::vector<std::string>& tokens,
              size_t index,
              CommandContext& ctx,
              std::string& err);
};
