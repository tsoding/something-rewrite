#ifndef SOMETHING_RENDERER_HPP_
#define SOMETHING_RENDERER_HPP_

#include "./something_triangle_vao.hpp"
#include "./something_program.hpp"

struct Game;

enum Shader_Asset: size_t {
    GRADIENT_CIRCLE_FRAG_SHADER_ASSET = 0,
    TEXTURE_COLOR_FRAG_SHADER_ASSET,
    TRIANGLE_CAMERA_VERT_SHADER_ASSET,
    HSL_FRAG_SHADER_ASSET,
    COUNT_SHADER_ASSETS,
};

enum Program_Asset: size_t {
    REGULAR_PROGRAM_ASSET = 0,
    PARTICLE_PROGRAM_ASSET,
    PRIDE_PROGRAM_ASSET,
    FONT_PROGRAM_ASSET,
    COUNT_PROGRAM_ASSETS
};

struct Batch {
    GLint first;
    GLsizei count;
    Program_Asset program;
};

struct Renderer {
    constexpr static size_t CAPACITY = 1024;

    Triangle_VAO triangle_vao;

    bool loaded;

    Shader shaders[COUNT_SHADER_ASSETS];
    Program programs[COUNT_PROGRAM_ASSETS];
    Batch batches[CAPACITY];
    size_t batch_size;

    void init();
    void clear();
    void draw(const Game *game);
    bool reload();

    Shader &get_shader(Index<Shader> shader_index);
    const Shader &get_shader(Index<Shader> shader_index) const;

    void batch_programs(Program_Asset program_asset, GLsizei count);

    void fill_triangle(Triangle<GLfloat> triangle,
                       RGBA rgba,
                       Triangle<GLfloat> uv,
                       Program_Asset program_asset);
    void fill_aabb(AABB<float> aabb, RGBA shade, AABB<float> uv_aabb,
                   Program_Asset program_asset);

};

#endif // SOMETHING_RENDERER_HPP_
