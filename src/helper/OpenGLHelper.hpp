#pragma once

#if defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>
#else 
#include <GL/gl.h>
#endif

#include <string>

namespace OpenGLHelper
{
    GLuint LoadTextureFromFile(
        const std::string& path,
        int* outWidth = nullptr,
        int* outHeight = nullptr
    );

    void DestroyTexture(GLuint texture);
}
