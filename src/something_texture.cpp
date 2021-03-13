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

RGBA32 Texture::get(int x, int y)
{
    assert(0 <= x && x < width);
    assert(0 <= y && y < height);
    return pixels[y * width + x];
}

void Texture::fill_cols(Texture src, int x0, AABB<int> aabb)
{
    const int x1 = aabb.pos.x;
    const int y1 = aabb.pos.y;
    const int x2 = aabb.pos.x + aabb.size.x - 1;
    const int y2 = aabb.pos.y + aabb.size.y - 1;

    assert(0 <= x1 && x1 < width);
    assert(0 <= y1 && y1 < height);
    assert(0 <= x2 && x2 < width);
    assert(0 <= y2 && y2 < height);

    for (int x = x1; x <= x2; ++x) {
        for (int y = y1; y <= y2; ++y) {
            pixels[y * width + x] = src.get(x0, y - aabb.pos.y);
        }
    }
}

void Texture::fill_rows(Texture src, int y0, AABB<int> aabb)
{
    const int x1 = aabb.pos.x;
    const int y1 = aabb.pos.y;
    const int x2 = aabb.pos.x + aabb.size.x - 1;
    const int y2 = aabb.pos.y + aabb.size.y - 1;

    assert(0 <= x1 && x1 < width);
    assert(0 <= y1 && y1 < height);
    assert(0 <= x2 && x2 < width);
    assert(0 <= y2 && y2 < height);

    for (int x = x1; x <= x2; ++x) {
        for (int y = y1; y <= y2; ++y) {
            pixels[y * width + x] = src.get(x - aabb.pos.x, y0);
        }
    }
}

void Texture::fill_aabb(AABB<int> aabb, RGBA32 color)
{
    const int x1 = aabb.pos.x;
    const int y1 = aabb.pos.y;
    const int x2 = aabb.pos.x + aabb.size.x - 1;
    const int y2 = aabb.pos.y + aabb.size.y - 1;

    assert(0 <= x1 && x1 < width);
    assert(0 <= y1 && y1 < height);
    assert(0 <= x2 && x2 < width);
    assert(0 <= y2 && y2 < height);

    for (int x = x1; x <= x2; ++x) {
        for (int y = y1; y <= y2; ++y) {
            pixels[y * width + x] = color;
        }
    }
}

void Texture::fill_texture(Texture src, V2<int> pos)
{
    const int x1 = pos.x;
    const int y1 = pos.y;
    const int x2 = pos.x + src.width  - 1;
    const int y2 = pos.y + src.height - 1;

    assert(0 <= x1 && x1 < width);
    assert(0 <= y1 && y1 < height);
    assert(0 <= x2 && x2 < width);
    assert(0 <= y2 && y2 < height);

    for (int x = x1; x <= x2; ++x) {
        for (int y = y1; y <= y2; ++y) {
            pixels[y * width + x] =
                src.pixels[(y - pos.y) * src.width + (x - pos.x)];
        }
    }
}

void Texture::fill_texture_with_margin(Texture src, int margin, V2<int> pos)
{
    // Check if the texture fits into the atlas
    {
        const int x1 = pos.x;
        const int y1 = pos.y;
        const int x2 = pos.x + src.width  + margin * 2 - 1;
        const int y2 = pos.y + src.height + margin * 2 - 1;

        assert(0 <= x1 && x1 < width);
        assert(0 <= y1 && y1 < height);
        assert(0 <= x2 && x2 < width);
        assert(0 <= y2 && y2 < height);
    }

    // Add the payload to the atlas
    fill_texture(src, pos + V2(margin));

    // Extending corners
    {
        const int CORNERS = 4;
        const int sxs[2] = {0, src.width - 1};
        const int sys[2] = {0, src.height - 1};
        const int bxs[2] = {pos.x, pos.x + src.width  + margin};
        const int bys[2] = {pos.y, pos.y + src.height + margin};

        for (int corner = 0; corner < CORNERS; ++corner) {
            const int ix = corner & 1;
            const int iy = (corner >> 1) & 1;
            fill_aabb(
                AABB(V2(bxs[ix], bys[iy]), V2(margin)),
                src.get(sxs[ix], sys[iy]));
        }
    }

    // Extending sides
    {
        // Top side
        {
            fill_rows(
                src,
                0,
                AABB(pos + V2(margin, 0), V2(src.width, margin)));
        }

        // Bottom side
        {
            fill_rows(
                src,
                src.height - 1,
                AABB(pos + V2(margin, margin + src.height), V2(src.width, margin)));
        }

        // Left side
        {
            fill_cols(
                src,
                0,
                AABB(pos + V2(0, margin), V2(margin, src.height)));
        }

        // Right side
        {
            fill_cols(
                src,
                src.width - 1,
                AABB(pos + V2(margin + src.width, margin), V2(margin, src.height)));
        }
    }
}

void Texture::save_to_png_file(const char *file_path)
{
    if (!stbi_write_png(file_path, width, height, 4, pixels, sizeof(RGBA32) * width)) {
        panic("Could not save texture to png file ", file_path, ": ", strerror(errno));
    }
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
