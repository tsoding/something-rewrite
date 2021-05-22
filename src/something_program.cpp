#include "./something_program.hpp"

bool Program::relink(Renderer *renderer)
{
    loaded = false;

    glDeleteProgram(this->id);

    const auto &vert_shader = renderer->get_shader(vert_shader_index);
    if (!vert_shader.loaded) {
        return loaded;
    }

    const auto &frag_shader = renderer->get_shader(frag_shader_index);
    if (!frag_shader.loaded) {
        return loaded;
    }

    this->id = glCreateProgram();
    glAttachShader(this->id, vert_shader.id);
    glAttachShader(this->id, frag_shader.id);
    glLinkProgram(this->id);

    GLint linked = GL_FALSE;
    glGetProgramiv(this->id, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        GLsizei log_size = 0;
        GLchar log[1024];
        glGetProgramInfoLog(this->id, sizeof(log), &log_size, log);

        const String_View log_sv = {(size_t) log_size, log};

        println(stderr, "Failed ot link program: ", log_sv);
        return loaded;
    }

    glUseProgram(this->id);

    u_image           = glGetUniformLocation(this->id, "image");
    u_resolution      = glGetUniformLocation(this->id, "resolution");
    u_camera_position = glGetUniformLocation(this->id, "camera_position");
    u_camera_zoom     = glGetUniformLocation(this->id, "camera_zoom");
    u_time            = glGetUniformLocation(this->id, "time");

    loaded = true;
    return loaded;
}

void Program::use() const
{
    glUseProgram(this->id);
}

void Program::sync_uniforms(const Game *game)
{
    glUniform2f(u_resolution, SCREEN_WIDTH, SCREEN_HEIGHT);
    glUniform2f(u_camera_position, game->camera.pos.x, game->camera.pos.y);
    glUniform1f(u_camera_zoom, game->camera.zoom);
    glUniform1f(u_time, game->time());
    glUniform1f(u_time, game->time());
}
