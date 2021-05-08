#ifndef SOMETHING_RENDERER_HPP_
#define SOMETHING_RENDERER_HPP_

#include "./something_triangle_vao.hpp"
#include "./something_circle_vao.hpp"
#include "./something_program.hpp"

struct Game;

struct Renderer {
    Triangle_VAO triangle_vao;
    Circle_VAO circle_vao;

    Program regular_program;
    Program particle_program;
    
    void init(Program regular_program, Program particle_program);
    void clear();
    void draw(const Game *game);
    void reload_programs();
    bool programs_failed() const;

    void fill_triangle(Triangle<GLfloat> triangle, RGBA rgba, Triangle<GLfloat> uv);
    void fill_aabb(AABB<float> aabb, RGBA shade, AABB<float> uv_aabb);
    void fill_circle(V2<GLfloat> center, GLfloat radius, RGBA color);
};

#endif // SOMETHING_RENDERER_HPP_
