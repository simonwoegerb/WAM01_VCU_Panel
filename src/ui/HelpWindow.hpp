#pragma once
#include "imgui.h"
#include <GLFW/glfw3.h>

#if defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>
#else 
#include <GL/gl.h>
#endif
class HelpWindow {
public:
    void Draw(bool* enable, GLuint* logo_texture);
};


