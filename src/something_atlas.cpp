#include "./something_texture.hpp"
#include "./something_atlas.hpp"

Atlas Atlas::from_config(const char *file_path, int margin)
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
            const char *texture_file_path = file_path_sv.as_cstr();
            assert(texture_file_path != nullptr);
            println(stdout, "INFO: loading ", texture_file_path, " ...");
            textures.push(Texture::from_file(texture_file_path));
        }
    }

    int atlas_width = 0;
    int atlas_height = 0;
    for (size_t i = 0; i < textures.size; ++i) {
        atlas_width = max(atlas_width, textures.data[i].width);
        atlas_height += textures.data[i].height;
    }
    atlas_width += 2 * margin;
    atlas_height += 2 * margin * static_cast<int>(textures.size);

    result.texture = Texture::from_solid_color(
                         atlas_width,
                         atlas_height,
                         0);

    int atlas_row = 0;
    for (size_t i = 0; i < textures.size; ++i) {
        result.texture.fill_texture_with_margin(
            textures.data[i],
            margin,
            V2(0, atlas_row));

        // Compute uvs of the texture
        {
            const int x = margin;
            const int y = atlas_row + margin;
            const int w = textures.data[i].width;
            const int h = textures.data[i].height;

            const float uv_x = static_cast<float>(x) / static_cast<float>(atlas_width);
            const float uv_y = static_cast<float>(y) / static_cast<float>(atlas_height);
            const float uv_w = static_cast<float>(w) / static_cast<float>(atlas_width);
            const float uv_h = static_cast<float>(h) / static_cast<float>(atlas_height);

            result.uvs.push(AABB(V2(uv_x, uv_y), V2(uv_w, uv_h)));
        }

        atlas_row += 2 * margin + textures.data[i].height;
    }

    result.gl_texture = GL_Texture::from_texture(result.texture);

    return result;
}

AABB<float> Atlas::get_uv(Index<AABB<float>> uv_index) const
{
    assert(uv_index.unwrap < uvs.size);
    return uvs.data[uv_index.unwrap];
}
