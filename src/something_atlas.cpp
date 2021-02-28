#include "./something_texture.hpp"
#include "./something_atlas.hpp"

Atlas Atlas::from_config(const char *file_path)
{
    Atlas result = {};

    String_View conf_content =
        unwrap_or_panic(
            read_file_as_string_view(file_path),
            "ERROR: could not read file ", file_path, ": ",
            strerror(errno));

    Dynamic_Array<Texture> textures = {};

    textures.push(Texture::from_solid_color(10, 10, 0xFFFFFFFF));

    while (conf_content.count > 0) {
        String_View line = conf_content.chop_by_delim('\n').trim();
        String_View file_path_sv = line.chop_by_delim('#').trim();

        if (file_path_sv.count > 0) {
            const char *file_path = file_path_sv.as_cstr();
            assert(file_path != nullptr);
            println(stdout, "INFO: loading ", file_path, " ...");
            textures.push(Texture::from_file(file_path));
        }
    }

    int atlas_width = 0;
    int atlas_height = 0;
    for (size_t i = 0; i < textures.size; ++i) {
        atlas_width = max(atlas_width, textures.data[i].width);
        atlas_height += textures.data[i].height;
    }

    const size_t atlas_pixels_size = sizeof(RGBA32) * atlas_width * atlas_height;
    RGBA32 *atlas_pixels = static_cast<RGBA32*>(malloc(atlas_pixels_size));
    memset(atlas_pixels, 0, atlas_pixels_size);

    int atlas_row = 0;
    for (size_t i = 0; i < textures.size; ++i) {
        const Texture *texture = &textures.data[i];

        const int x = 0;
        const int y = atlas_row;
        const int w = texture->width;
        const int h = texture->height;

        const float uv_x = static_cast<float>(x) / static_cast<float>(atlas_width);
        const float uv_y = static_cast<float>(y) / static_cast<float>(atlas_height);
        const float uv_w = static_cast<float>(w) / static_cast<float>(atlas_width);
        const float uv_h = static_cast<float>(h) / static_cast<float>(atlas_height);

        result.uvs.push(AABB(V2(uv_x, uv_y), V2(uv_w, uv_h)));

        for (int row = 0; row < textures.data[i].height; ++row) {
            memcpy(&atlas_pixels[atlas_row * atlas_width],
                   &texture->pixels[row * texture->width],
                   sizeof(RGBA32) * texture->width);
            atlas_row += 1;
        }
    }

    result.texture = Texture::from_memory(atlas_width, atlas_height, atlas_pixels);
    result.gl_texture = GL_Texture::from_texture(result.texture);

    return result;
}
