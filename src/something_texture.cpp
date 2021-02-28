#include "./something_texture.hpp"

Texture Texture::from_file(const char *file_path)
{
    int width, height;
    RGBA32 *const pixels =
        reinterpret_cast<RGBA32*>(
            stbi_load(
                file_path,
                &width,
                &height,
                NULL, 4));
    if (pixels == NULL) {
        panic("ERROR: could not load file `", file_path, "`");
    }

    return Texture::from_memory(width, height, pixels);
}

Texture Texture::from_solid_color(int width, int height, RGBA32 color)
{
    RGBA32 *const pixels =
        static_cast<RGBA32*>(
            malloc(sizeof(RGBA32) * width * height));
    for (int i = 0; i < width * height; ++i) {
        pixels[i] = color;
    }
    return Texture::from_memory(width, height, pixels);
}

Texture Texture::from_memory(int width, int height, RGBA32 *pixels)
{
    Texture texture = {};
    texture.width = width;
    texture.height = height;
    texture.pixels = pixels;

    return texture;
}

GL_Texture GL_Texture::from_texture(Texture texture)
{
    GL_Texture result = {};

    glGenTextures(1, &result.id);
    glBindTexture(GL_TEXTURE_2D, result.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 texture.width,
                 texture.height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 texture.pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    // glActiveTexture(GL_TEXTURE0);

    return result;
}
