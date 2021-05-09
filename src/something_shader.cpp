#include "./something_shader.hpp"

bool Shader::reload()
{
    loaded = false;

    const auto maybe_source = read_file_as_string_view(this->file_path);
    if (!maybe_source.has_value) {
        println(stderr, "Could not read shader file `", this->file_path, "`: ",
                strerror(errno));
        return loaded;
    }
    const auto source = maybe_source.unwrap;
    defer(destroy(source));

    const GLint source_size = static_cast<GLint>(source.count);

    this->id = glCreateShader(this->type);
    glShaderSource(this->id, 1, &source.data, &source_size);
    glCompileShader(this->id);

    GLint compiled = GL_FALSE;
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        GLchar log[1024];
        GLsizei log_size = 0;
        glGetShaderInfoLog(this->id, sizeof(log), &log_size, log);

        const String_View log_sv = {(size_t) log_size, log};

        println(stderr, "Failed to compile `", file_path, "`:", log_sv);
        return loaded;
    }

    loaded = true;
    return loaded;
}
