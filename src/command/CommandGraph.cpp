#include "CommandGraph.hpp"
#include <sstream>

std::vector<std::string> CommandGraph::Tokenize(const std::string& s)
{
    std::stringstream ss(s);
    std::vector<std::string> out;
    std::string t;

    while (ss >> t)
        out.push_back(t);

    return out;
}

bool CommandGraph::Execute(
    const std::string& input,
    std::function<void(const std::string&)> logger,
    std::string& err)
{
    auto tokens = Tokenize(input);

    CommandContext ctx;
    ctx.log = logger;   // 🔥 THIS is the missing link

    return Walk(&root, tokens, 0, ctx, err);
}

bool CommandGraph::Walk(CommandNode* node,
                        const std::vector<std::string>& tokens,
                        size_t index,
                        CommandContext& ctx,
                        std::string& err)
{
    if (index >= tokens.size())
    {
        if (node->executor)
        {
            node->executor(ctx);
            return true;
        }

        err = "Incomplete command";
        return false;
    }

    const std::string& tok = tokens[index];

    for (auto& child : node->children)
    {
        // literal match
        if (child->type == CommandNode::Type::Literal &&
            child->name == tok)
        {
            return Walk(child.get(), tokens, index + 1, ctx, err);
        }

        // argument parse
        if (child->type == CommandNode::Type::Argument)
        {
            Value v;
            if (child->arg.parse(tok, v))
            {
                ctx.args[child->arg.name] = v;
                return Walk(child.get(), tokens, index + 1, ctx, err);
            }
        }
    }

    err = "Unknown token: " + tok;
    return false;
}
std::vector<std::string> CommandGraph::Complete(const std::string& input)
{
    auto tokens = Tokenize(input);

    CommandNode* current = &root;

    for (size_t i = 0; i < tokens.size(); i++)
    {
        bool found = false;

        for (auto& child : current->children)
        {
            if (child->type == CommandNode::Type::Literal &&
                child->name == tokens[i])
            {
                current = child.get();
                found = true;
                break;
            }
        }

        if (!found)
            break;
    }

    std::vector<std::string> out;

    for (auto& child : current->children)
        out.push_back(child->name);

    return out;
}
