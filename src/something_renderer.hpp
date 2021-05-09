#ifndef SOMETHING_RENDERER_HPP_
#define SOMETHING_RENDERER_HPP_

#include "./something_triangle_vao.hpp"
#include "./something_circle_vao.hpp"
#include "./something_program.hpp"

struct Game;

enum Shader_Asset: size_t {
    CIRCLE_VERT_SHADER_ASSET = 0,
    PARTICLE_FRAG_SHADER_ASSET,
    RECT_FRAG_SHADER_ASSET,
    RECT_VERT_SHADER_ASSET,
    COUNT_SHADER_ASSETS,
};

enum Program_Asset: size_t {
    REGULAR_PROGRAM_ASSET = 0,
    PARTICLE_PROGRAM_ASSET,
    COUNT_PROGRAM_ASSETS
};

struct Renderer {
    Triangle_VAO triangle_vao;
    Circle_VAO circle_vao;

    bool loaded;

    Shader shaders[COUNT_SHADER_ASSETS];
    Program programs[COUNT_PROGRAM_ASSETS];

    void init();
    void clear();
    void draw(const Game *game);
    bool reload();

    Shader &get_shader(Index<Shader> shader_index);
    const Shader &get_shader(Index<Shader> shader_index) const;

    void fill_triangle(Triangle<GLfloat> triangle,
                       RGBA rgba,
                       Triangle<GLfloat> uv);
    void fill_aabb(AABB<float> aabb, RGBA shade, AABB<float> uv_aabb);
    void fill_circle(V2<GLfloat> center, GLfloat radius, RGBA color);
};

#endif // SOMETHING_RENDERER_HPP_
