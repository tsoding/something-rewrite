#ifndef SOMETHING_TEXTURE_HPP_
#define SOMETHING_TEXTURE_HPP_

#include <stdint.h>

using RGBA32 = uint32_t;

struct Texture {
    int width;
    int height;
    RGBA32 *pixels;

    static Texture from_file(const char *file_path);
    static Texture from_memory(int width, int height, RGBA32 *pixels);
    static Texture from_solid_color(int width, int height, RGBA32 color);
};

struct GL_Texture {
    GLuint id;

    static GL_Texture from_texture(Texture texture);
};

#endif  // SOMETHING_TEXTURE_HPP_
