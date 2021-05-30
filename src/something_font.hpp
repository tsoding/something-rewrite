#ifndef SOMETHING_FONT_HPP_
#define SOMETHING_FONT_HPP_

struct Font {
    constexpr static size_t ROWS = 7;
    constexpr static size_t COLS = 18;

    Texture texture;
    GL_Texture gl_texture;
    V2<size_t> tex_size_pix;
    V2<size_t> char_size_pix;
    V2<GLfloat> char_size_uv;

    static Font from_file(const char *file_path);
    AABB<GLfloat> char_uv(char c) const;
    
    V2<GLfloat> text_size(const char *text, GLfloat scale) const;

    void render_text(Renderer *renderer,
                     const char *text,
                     V2<GLfloat> position,
                     GLfloat scale,
                     RGBA color) const;
};

#endif // SOMETHING_FONT_HPP_
