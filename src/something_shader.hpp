#ifndef SOMETHING_SHADER_HPP_
#define SOMETHING_SHADER_HPP_

struct Shader {
    GLuint id;
    GLenum type;
    const char *file_path;
    bool loaded;

    static constexpr Shader make(GLenum shader_type, const char *file_path)
    {
        Shader self = {};
        self.file_path = file_path;
        self.type = shader_type;
        return self;
    }

    bool reload();
};

#endif // SOMETHING_SHADER_HPP_
