#ifndef SOMETHING_PROGRAM_HPP_
#define SOMETHING_PROGRAM_HPP_

#include "./something_shader.hpp"

struct Renderer;
struct Game;

struct Program {
    bool loaded;

    GLuint id;

    Index<Shader> vert_shader_index;
    Index<Shader> frag_shader_index;

    GLuint u_image;
    GLuint u_resolution;
    GLuint u_time;
    GLuint u_camera_position;
    GLuint u_camera_zoom;

    int image_unit;

    constexpr static Program make(Index<Shader> vert_shader_index,
                                  Index<Shader> frag_shader_index,
                                  int image_unit = 0)
    {
        Program self = {};
        self.vert_shader_index = vert_shader_index;
        self.frag_shader_index = frag_shader_index;
        self.image_unit = image_unit;
        return self;
    }

    bool relink(Renderer *renderer);
    void use() const;
    void sync_uniforms(const Game *game);
};

#endif // SOMETHING_PROGRAM_HPP_
