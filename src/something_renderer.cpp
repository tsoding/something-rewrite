#include "./something_renderer.hpp"

void Renderer::init()
{
    triangle_vao.init();

    shaders[PARTICLE_FRAG_SHADER_ASSET] =
        Shader::make(GL_FRAGMENT_SHADER, "./assets/shaders/particle.frag");
    shaders[RECT_FRAG_SHADER_ASSET] =
        Shader::make(GL_FRAGMENT_SHADER, "./assets/shaders/rect.frag");
    shaders[RECT_VERT_SHADER_ASSET] =
        Shader::make(GL_VERTEX_SHADER, "./assets/shaders/rect.vert");
    shaders[HSL_FRAG_SHADER_ASSET] =
        Shader::make(GL_FRAGMENT_SHADER, "./assets/shaders/hsl.frag");
    static_assert(COUNT_SHADER_ASSETS == 4);

    programs[REGULAR_PROGRAM_ASSET] =
        Program::make({RECT_VERT_SHADER_ASSET}, {RECT_FRAG_SHADER_ASSET});
    programs[PARTICLE_PROGRAM_ASSET] =
        Program::make({RECT_VERT_SHADER_ASSET}, {PARTICLE_FRAG_SHADER_ASSET});
    programs[PRIDE_PROGRAM_ASSET] =
        Program::make({RECT_VERT_SHADER_ASSET}, {HSL_FRAG_SHADER_ASSET});
    static_assert(COUNT_PROGRAM_ASSETS == 3);
}

void Renderer::clear()
{
    triangle_vao.clear();
    batch_size = 0;
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

        for (size_t i = 0; i < COUNT_PROGRAM_ASSETS; ++i) {
            programs[i].use();
            programs[i].sync_uniforms(game);
        }

        triangle_vao.use();
        triangle_vao.sync_buffers();
        for (size_t i = 0; i < batch_size; ++i) {
            const auto &batch = batches[i];
            programs[batch.program].use();
            glDrawArrays(
                GL_TRIANGLES,
                batch.first * TRIANGLE_VERT_COUNT,
                batch.count * TRIANGLE_VERT_COUNT);
        }
    } else {
        glClearColor(FAILED_BACKGROUND_COLOR.r,
                     FAILED_BACKGROUND_COLOR.g,
                     FAILED_BACKGROUND_COLOR.b,
                     FAILED_BACKGROUND_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Renderer::batch_programs(Program_Asset program_asset, GLsizei count)
{
    if (batch_size == 0) {
        assert(batch_size < CAPACITY);
        batches[batch_size].program = program_asset;
        batches[batch_size].first = 0;
        batches[batch_size].count = 0;
        batch_size += 1;
    }

    if (batches[batch_size - 1].program != program_asset) {
        assert(batch_size < CAPACITY);
        batches[batch_size].program = program_asset;
        batches[batch_size].first =
            batches[batch_size - 1].first + batches[batch_size - 1].count;
        batches[batch_size].count = 0;
        batch_size += 1;
    }

    batches[batch_size - 1].count += count;
}

void Renderer::fill_triangle(Triangle<GLfloat> triangle, RGBA rgba, Triangle<GLfloat> uv,
                             Program_Asset program_asset)
{
    triangle_vao.fill_triangle(triangle, rgba, uv);
    batch_programs(program_asset, 1);
}

void Renderer::fill_aabb(AABB<float> aabb, RGBA shade, AABB<float> uv_aabb,
                         Program_Asset program_asset)
{
    triangle_vao.fill_aabb(aabb, shade, uv_aabb);
    batch_programs(program_asset, 2);
}

void Renderer::fill_circle(V2<GLfloat> center, GLfloat radius, RGBA color,
                           Program_Asset program_asset)
{
    fill_aabb(AABB(center - V2(radius), V2(radius) * 2.0f),
             color,
             AABB(V2(0.0f), V2(1.0f)),
             program_asset);
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

