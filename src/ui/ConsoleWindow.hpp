#pragma once
#include "command/CommandGraph.hpp"
#include <vector>
#include <string>

class ConsoleWindow
{
public:
    ConsoleWindow(CommandGraph& graph);

    void Draw(bool* open);

private:
    CommandGraph& m_graph;

    char m_input[256]{};

    std::vector<std::string> m_log;
    std::vector<std::string> m_history;

    int m_historyIndex = -1;

    std::vector<std::string> m_suggestions;

    void AddLog(const std::string& msg);
    void UpdateAutocomplete();
    void BindContext(CommandContext& ctx);
};
