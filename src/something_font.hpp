#ifndef SOMETHING_FONT_HPP_
#define SOMETHING_FONT_HPP_

struct Font
{
    Texture texture;
    GL_Texture gl_texture;

    static Font from_file(const char *file_path);
};

#endif // SOMETHING_FONT_HPP_
