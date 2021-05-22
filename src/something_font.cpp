#include "./something_font.hpp"

Font Font::from_file(const char *file_path)
{
    Font result = {};
    result.texture = Texture::from_file(file_path);
    result.gl_texture = GL_Texture::from_texture(result.texture, GL_TEXTURE0 + 1);
    return result;
}
