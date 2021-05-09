#ifndef SOMETHING_SHADER_HPP_
#define SOMETHING_SHADER_HPP_

struct Shader {
    GLuint id;
    GLenum type;
    const char *file_path;
    bool failed;

    static Shader load_from_file(const char *file_path,
                                 GLenum shader_type);
    bool reload();
};

#endif // SOMETHING_SHADER_HPP_
