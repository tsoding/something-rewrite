#include "./something_font.hpp"

Font Font::from_file(const char *file_path)
{
    Font self = {};
    self.texture = Texture::from_file(file_path);
    for (int y = 0; y < self.texture.height; ++y) {
        for (int x = 0; x < self.texture.width; ++x) {
            RGBA32 *pixel = &self.texture.pixels[y * self.texture.width + x];
            if (*pixel == 0xFF000000) {
                *pixel = 0;
            }
        }
    }

    self.gl_texture = GL_Texture::from_texture(
                          self.texture,
                          GL_TEXTURE0 + 1,
                          true);
    self.tex_size_pix = V2(self.texture.width, self.texture.height).cast_to<size_t>();
    const auto tex_size_cell = V2(COLS, ROWS);
    self.char_size_pix = self.tex_size_pix / tex_size_cell;
    self.char_size_uv = self.char_size_pix.cast_to<GLfloat>() / self.tex_size_pix.cast_to<GLfloat>();
    return self;
}

AABB<GLfloat> Font::char_uv(char c) const
{
    if (32 <= c && c <= 126) {
        // TODO(#89): Font::char_uv should be implementing entirely in a shader
        const size_t char_index  = c - 32;
        const auto char_pos_cell = V2(char_index % COLS, char_index / COLS);
        const auto char_pos_pix  = char_pos_cell * char_size_pix;
        const auto char_pos_uv   = char_pos_pix.cast_to<GLfloat>() / tex_size_pix.cast_to<GLfloat>();

        return AABB(char_pos_uv, char_size_uv).flip_vertically();
    }

    constexpr char INVALID = '?';
    static_assert(32 <= INVALID && INVALID <= 126);
    return char_uv(INVALID);
}

void Font::render_text(Renderer *renderer, const char *text, V2<GLfloat> position, GLfloat scale, RGBA color) const
{
    const size_t n = strlen(text);
    for (size_t i = 0; i < n; ++i) {
        renderer->fill_aabb(
            AABB(position + (char_size_pix * V2<size_t>(i, 0)).cast_to<GLfloat>() * V2(scale),
                 char_size_pix.cast_to<GLfloat>() * V2(scale)),
            color,
            char_uv(text[i]),
            FONT_PROGRAM_ASSET);
    }
}
