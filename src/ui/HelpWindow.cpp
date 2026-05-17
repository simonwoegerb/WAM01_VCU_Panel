#include "ui/HelpWindow.hpp"

void HelpWindow::Draw(bool* enabled, GLuint* logo_texture)  {
    ImGui::Begin("Help", enabled);
    ImGui::Image(
        (ImTextureID)(intptr_t)(*logo_texture),
        ImVec2(1370/4,402/4)
    );

    ImGui::Text("This application was developed by Simon Wögerbauer");
    ImGui::Text("See the project on Github:");
    ImGui::SameLine();
    ImGui::TextLinkOpenURL("simonwoegerb/WAM01_VCU_Panel","https://github.com/simonwoegerb/WAM01_VCU_Panel");
    ImGui::Text("© 2026 FH OÖ Racing Team. All rights reserved.");
    ImGui::End();

}
