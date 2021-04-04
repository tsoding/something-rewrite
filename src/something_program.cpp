#include "./something_program.hpp"

static Fixed_Region<10 * 1000 * 1000> shader_buffer = {};

static bool gl_compile_shader_file(const char *file_path, GLenum shader_type, GLuint *shader)
{
    const auto source = unwrap_or_panic(
                            read_file_as_string_view(file_path, &shader_buffer),
                            "Could not read file `", file_path, "`: ",
                            strerror(errno));
    const GLint source_size = static_cast<GLint>(source.count);

    *shader = glCreateShader(shader_type);
    glShaderSource(*shader, 1, &source.data, &source_size);
    glCompileShader(*shader);

    GLint compiled;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        GLchar log[1024];
        GLsizei log_size = 0;
        glGetShaderInfoLog(*shader, sizeof(log), &log_size, log);

        const String_View log_sv = {
            (size_t) log_size,
            log
        };

        println(stderr, "Failed to compile ", file_path, ":", log_sv);
        return false;
    }

    return true;
}

static bool gl_link_program(GLuint *shader, size_t shader_size, GLuint *program)
{
    *program = glCreateProgram();

    for (size_t i = 0; i < shader_size; ++i) {
        glAttachShader(*program, shader[i]);
    }
    glLinkProgram(*program);

    GLint linked = 0;
    glGetProgramiv(*program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        GLsizei log_size = 0;
        GLchar log[1024];
        glGetProgramInfoLog(*program, sizeof(log), &log_size, log);

        const String_View log_sv = {
            (size_t) log_size,
            log
        };

        panic("Failed to link the shader program: ", log_sv);
    }

    return program;
}


Program Program::load_from_shader_files(const char *vert_shader_path,
                                        const char *frag_shader_path)
{
    Program result = {};
    result.vert_shader_path = vert_shader_path;
    result.frag_shader_path = frag_shader_path;
    result.reload();
    return result;
}

bool Program::reload()
{
    failed = true;

    println(stderr, "LOG: compiling the shader program");
    glDeleteProgram(program);
    // Compiling The Shader Program
    {
        GLuint shaders[2] = {0};

        if (!gl_compile_shader_file(vert_shader_path, GL_VERTEX_SHADER, shaders)) {
            return false;
        }

        if (!gl_compile_shader_file(frag_shader_path, GL_FRAGMENT_SHADER, shaders + 1)) {
            return false;
        }

        if (!gl_link_program(shaders, sizeof(shaders) / sizeof(shaders[0]), &program)) {
            return false;
        }
    }
    glUseProgram(program);

    // Uniforms
    u_atlas           = glGetUniformLocation(program, "atlas");
    u_resolution      = glGetUniformLocation(program, "resolution");
    u_camera_position = glGetUniformLocation(program, "camera_position");
    u_camera_scale    = glGetUniformLocation(program, "camera_scale");
    u_time            = glGetUniformLocation(program, "time");

    failed = false;

    return true;
}

void Program::use() const
{
    glUseProgram(program);
}
