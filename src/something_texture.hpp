#ifndef SOMETHING_TEXTURE_HPP_
#define SOMETHING_TEXTURE_HPP_

#include <stdint.h>

#include "./something_math.hpp"

using RGBA32 = uint32_t;

struct Texture {
    int width;
    int height;
    RGBA32 *pixels;

    RGBA32 get(int x, int y);

    static Texture from_file(const char *file_path);
    static Texture from_memory(int width, int height, RGBA32 *pixels);
    static Texture from_solid_color(int width, int height, RGBA32 color);

    void fill_cols(Texture src, int x0, AABB<int> aabb);
    void fill_rows(Texture src, int y0, AABB<int> aabb);
    void fill_aabb(AABB<int> aabb, RGBA32 color);
    void fill_texture(Texture src, V2<int> pos);
    void fill_texture_with_margin(Texture src, int margin, V2<int> pos);

    void save_to_png_file(const char *file_path);
};

struct GL_Texture {
    GLuint id;

    static GL_Texture from_texture(Texture texture, GLenum unit);
};

#endif  // SOMETHING_TEXTURE_HPP_
