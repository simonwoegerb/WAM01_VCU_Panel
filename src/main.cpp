#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "helper/OpenGLHelper.hpp"

#include "command/CommandGraph.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <GLFW/glfw3.h>

#include "ui/ConsoleWindow.hpp"
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>
#else 
#include <GL/gl.h>
#endif

int main() {
    // -----------------------
    // Init GLFW
    // -----------------------
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "WAM01 VCU Panel", NULL, NULL);
    if (!window)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

#if defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return 1;
    }
#endif

    // -----------------------
    // Setup ImGui context
    // -----------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    // Backend init
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    GLuint logo_texture = OpenGLHelper::LoadTextureFromFile("src/resources/logo.jpg");
   // -------------------------
   CommandGraph graph;
   auto hv = std::make_unique<CommandNode>("hv", CommandNode::Type::Literal);

auto enable = std::make_unique<CommandNode>("enable", CommandNode::Type::Literal);

enable->executor = [](CommandContext& ctx)
{
	std::cout << "HV" << std::endl;
    ctx.Log("HV ENABLED");
};

hv->add(std::move(enable));
graph.root.add(std::move(hv));

ConsoleWindow console(graph);
    // -----------------------
    // Main loop
    // -----------------------
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplGlfw_NewFrame();
ImGui::NewFrame();

// =========================
// GLOBAL STATE
// =========================
static bool show_console = true;
static bool show_help = false;

// =========================
// TOP MENU BAR
// =========================
if (ImGui::BeginMainMenuBar())
{
	    //ImGui::Image( (ImTextureID)(intptr_t)logo_texture, ImVec2(1370/15, 402/15));
    if (ImGui::Button("Console"))
    {
	show_console = !show_console;
    }
    if (ImGui::Button("Help"))
    {
	    show_help = !show_help;
    }

    ImGui::EndMainMenuBar();
}

// =========================
// SUBMENU WINDOW
// =========================
if (show_help)
{
    ImGui::Begin("Help", &show_help);
    ImGui::Image(
        (ImTextureID)(intptr_t)logo_texture,
        ImVec2(1370/4,402/4)
    );

    ImGui::Text("This application was developed by Simon Wögerbauer");
    ImGui::Text("See the project on Github:");
    ImGui::SameLine();
    ImGui::TextLinkOpenURL("simonwoegerb/WAM01_VCU_Panel","https://github.com/simonwoegerb/WAM01_VCU_Panel");
    ImGui::Text("© 2026 FH OÖ Racing Team. All rights reserved.");
    ImGui::End();
}

// =========================
// CONSOLE WINDOW
// =========================
if (show_console)
{
 console.Draw(&show_console);
}


// =========================
// RENDER
// =========================
ImGui::Render();

int display_w, display_h;
glfwGetFramebufferSize(window, &display_w, &display_h);
glViewport(0, 0, display_w, display_h);
glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT);

ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // Start frame
// =========================
// RENDER
// =========================

        glfwSwapBuffers(window);
    }

    // -----------------------
    // Cleanup
    // -----------------------
    OpenGLHelper::DestroyTexture(logo_texture);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
