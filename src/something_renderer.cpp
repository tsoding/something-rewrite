#include "./something_renderer.hpp"

void Renderer::init()
{
    triangle_vao.init();
    circle_vao.init();

    shaders[CIRCLE_VERT_SHADER_ASSET] =
        Shader::make(GL_VERTEX_SHADER, "./assets/shaders/circle.vert");
    shaders[PARTICLE_FRAG_SHADER_ASSET] =
        Shader::make(GL_FRAGMENT_SHADER, "./assets/shaders/particle.frag");
    shaders[RECT_FRAG_SHADER_ASSET] =
        Shader::make(GL_FRAGMENT_SHADER, "./assets/shaders/rect.frag");
    shaders[RECT_VERT_SHADER_ASSET] =
        Shader::make(GL_VERTEX_SHADER, "./assets/shaders/rect.vert");
    static_assert(COUNT_SHADER_ASSETS == 4);

    programs[REGULAR_PROGRAM_ASSET] =
        Program::make({RECT_VERT_SHADER_ASSET}, {RECT_FRAG_SHADER_ASSET});
    programs[PARTICLE_PROGRAM_ASSET] =
        Program::make({CIRCLE_VERT_SHADER_ASSET}, {PARTICLE_FRAG_SHADER_ASSET});
    static_assert(COUNT_PROGRAM_ASSETS == 2);
}

void Renderer::clear()
{
    triangle_vao.clear();
    circle_vao.clear();
}

bool Renderer::reload()
{
    loaded = false;

    for (size_t i = 0; i < COUNT_SHADER_ASSETS; ++i) {
        if (!shaders[i].reload()) {
            return loaded;
        }
    }

    for (size_t i = 0; i < COUNT_PROGRAM_ASSETS; ++i) {
        if (!programs[i].relink(this)) {
            return loaded;
        }
    }

    loaded = true;
    return loaded;
}

void Renderer::draw(const Game *game)
{
    if (loaded) {
        glClearColor(BACKGROUND_COLOR.r,
                     BACKGROUND_COLOR.g,
                     BACKGROUND_COLOR.b,
                     BACKGROUND_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT);

        // Triangles
        {
            triangle_vao.use();
            triangle_vao.sync_buffers();
            programs[REGULAR_PROGRAM_ASSET].use();
            programs[REGULAR_PROGRAM_ASSET].sync_uniforms(game);
            triangle_vao.draw();
        }

        // Circles
        {
            circle_vao.use();
            circle_vao.sync_buffers();
            programs[PARTICLE_PROGRAM_ASSET].use();
            programs[PARTICLE_PROGRAM_ASSET].sync_uniforms(game);
            circle_vao.draw();
        }
    } else {
        glClearColor(FAILED_BACKGROUND_COLOR.r,
                     FAILED_BACKGROUND_COLOR.g,
                     FAILED_BACKGROUND_COLOR.b,
                     FAILED_BACKGROUND_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Renderer::fill_triangle(Triangle<GLfloat> triangle, RGBA rgba, Triangle<GLfloat> uv)
{
    triangle_vao.fill_triangle(triangle, rgba, uv);
}

void Renderer::fill_aabb(AABB<float> aabb, RGBA shade, AABB<float> uv_aabb)
{
    triangle_vao.fill_aabb(aabb, shade, uv_aabb);
}

void Renderer::fill_circle(V2<GLfloat> center, GLfloat radius, RGBA color)
{
    circle_vao.fill_circle(center, radius, color);
}

Shader &Renderer::get_shader(Index<Shader> index)
{
    assert(index.unwrap < COUNT_SHADER_ASSETS);
    return shaders[index.unwrap];
}

const Shader &Renderer::get_shader(Index<Shader> index) const
{
    assert(index.unwrap < COUNT_SHADER_ASSETS);
    return shaders[index.unwrap];
}

