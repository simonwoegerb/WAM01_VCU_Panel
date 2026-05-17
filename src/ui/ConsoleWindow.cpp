#include "ConsoleWindow.hpp"
#include "imgui.h"

ConsoleWindow::ConsoleWindow(CommandGraph& graph)
    : m_graph(graph)
{}

void ConsoleWindow::AddLog(const std::string& msg)
{
    m_log.push_back(msg);
}

void ConsoleWindow::UpdateAutocomplete()
{
    m_suggestions = m_graph.Complete(m_input);
}
void ConsoleWindow::Draw(bool* open)
{
    if (!ImGui::Begin("Console", open))
    {
        ImGui::End();
        return;
    }

    // =========================
    // LOG OUTPUT
    // =========================
    ImGui::BeginChild("log", ImVec2(0, -60), true);

    for (const auto& line : m_log)
        ImGui::TextUnformatted(line.c_str());

    ImGui::EndChild();

    // =========================
    // INPUT
    // =========================
    bool enterPressed = ImGui::InputText(
        "##cmd",
        m_input,
        sizeof(m_input),
        ImGuiInputTextFlags_EnterReturnsTrue
    );


    if (enterPressed)
{
    std::string err;

    // 1. print command first (CLI style)
    AddLog(std::string("> ") + m_input);

    // 2. execute command (may generate logs after)
    bool ok = m_graph.Execute(
        m_input,
        [&](const std::string& msg)
        {
            AddLog(msg);
        },
        err
    );

    if (!ok)
        AddLog("[ERR] " + err);

    m_input[0] = '\0';
    m_suggestions.clear();
}
    // =========================
    // AUTOCOMPLETE
    // =========================
    UpdateAutocomplete();


    if (!m_suggestions.empty())
{
    ImGui::Separator();

    for (const auto& s : m_suggestions)
    {
        if (ImGui::Selectable(s.c_str()))
        {
            std::string input(m_input);

            // find last space (current token start)
            size_t pos = input.find_last_of(" ");

            if (pos == std::string::npos)
            {
                // first token → replace whole input
                input = s;
            }
            else
            {
                // replace only last token
                input = input.substr(0, pos + 1) + s;
            }

            strncpy(m_input, input.c_str(), sizeof(m_input));
            m_input[sizeof(m_input) - 1] = '\0';
        }
    }
}
    ImGui::End();
}
