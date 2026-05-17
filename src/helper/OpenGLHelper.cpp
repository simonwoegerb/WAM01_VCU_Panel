#include "helper/OpenGLHelper.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../../3rd_party/stb_image.h"

#include <iostream>

namespace OpenGLHelper
{
    GLuint LoadTextureFromFile(
        const std::string& path,
        int* outWidth,
        int* outHeight
    )
    {
        int width = 0;
        int height = 0;
        int channels = 0;

        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(
            path.c_str(),
            &width,
            &height,
            &channels,
            STBI_rgb_alpha
        );

        if (!data)
        {
            std::cerr
                << "Failed to load texture: "
                << path
                << std::endl;

            return 0;
        }

        GLuint texture = 0;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR
        );

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data
        );


        stbi_image_free(data);

        if (outWidth)
            *outWidth = width;

        if (outHeight)
            *outHeight = height;

        return texture;
    }

    void DestroyTexture(GLuint texture)
    {
        if (texture != 0)
        {
            glDeleteTextures(1, &texture);
        }
    }
}
