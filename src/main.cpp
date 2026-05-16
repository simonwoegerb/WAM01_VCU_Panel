#define STB_IMAGE_IMPLEMENTATION
#include "../3rd_party/stb_image.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <cstring>
#include <GLFW/glfw3.h>

#if defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>
#endif
GLuint LoadTextureFromFile(const char* path)
{
    int width, height, channels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data =
        stbi_load(path, &width, &height, &channels, 4);

    if (!data)
    {
        printf("Failed to load image: %s\n", path);
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);

    // IMPORTANT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    stbi_image_free(data);

    return texture;
}

int main() {
    // -----------------------
    // Init GLFW
    // -----------------------
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui App", NULL, NULL);
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
    GLuint logo_texture =
    LoadTextureFromFile("resources/logo.jpg");

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
static bool show_submenu = true;

// =========================
// TOP MENU BAR
// =========================
if (ImGui::BeginMainMenuBar())
{
	    ImGui::Image(
        (ImTextureID)(intptr_t)logo_texture,
        ImVec2(24, 24)
    );
    if (ImGui::BeginMenu("Windows"))
    {

        ImGui::MenuItem("Submenu", nullptr, &show_submenu);
        ImGui::MenuItem("Console", nullptr, &show_console);
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

// =========================
// SUBMENU WINDOW
// =========================
if (show_submenu)
{
    ImGui::Begin("Submenu", &show_submenu);

    ImGui::Text("This is a separate window");

    static float slider = 0.5f;
    ImGui::SliderFloat("Values", &slider, 0.0f, 1.0f);

    ImGui::End();
}

// =========================
// CONSOLE WINDOW
// =========================
if (show_console)
{
    ImGui::Begin("Console", &show_console);

    static char input[256] = "";
    static std::vector<std::string> log;

    // -------------------------
    // Output
    // -------------------------
    ImGui::BeginChild("ConsoleOutput", ImVec2(0, -35), true);

    for (auto& line : log)
        ImGui::TextUnformatted(line.c_str());

    ImGui::EndChild();

    // -------------------------
    // Input
    // -------------------------
    bool submit = false;

    if (ImGui::InputText("##cmd", input, IM_ARRAYSIZE(input),
        ImGuiInputTextFlags_EnterReturnsTrue))
    {
        submit = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("Enter"))
        submit = true;

    if (submit && input[0] != '\0')
    {
        log.push_back(std::string("> ") + input);

        if (strcmp(input, "clear") == 0)
            log.clear();

        input[0] = '\0';
    }

    ImGui::End();
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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
