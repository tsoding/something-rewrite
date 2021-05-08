#include "./something_renderer.hpp"

void Renderer::init(Program regular_program, Program particle_program)
{
    this->regular_program = regular_program;
    this->particle_program = particle_program;

    triangle_vao.init();
    circle_vao.init();
}

void Renderer::clear()
{
    triangle_vao.clear();
    circle_vao.clear();
}

void Renderer::reload_programs()
{
    regular_program.reload();
    particle_program.reload();
}

bool Renderer::programs_failed() const
{
    return regular_program.failed || particle_program.failed;
}

void Renderer::draw(const Game *game)
{
    if (!regular_program.failed && !particle_program.failed) {
        // Triangles
        {
            triangle_vao.use();
            triangle_vao.sync_buffers();

            regular_program.use();
            glUniform2f(regular_program.u_resolution, SCREEN_WIDTH, SCREEN_HEIGHT);
            glUniform2f(regular_program.u_camera_position, game->camera.pos.x, game->camera.pos.y);
            glUniform1f(regular_program.u_camera_zoom, game->camera.zoom);
            glUniform1f(regular_program.u_time, game->time());

            glUniform1f(regular_program.u_time, game->time());

            triangle_vao.draw();
        }

        // Circles
        {
            circle_vao.use();
            circle_vao.sync_buffers();

            particle_program.use();
            glUniform2f(regular_program.u_resolution, SCREEN_WIDTH, SCREEN_HEIGHT);
            glUniform2f(regular_program.u_camera_position,
                        game->camera.pos.x,
                        game->camera.pos.y);
            glUniform1f(regular_program.u_camera_zoom, game->camera.zoom);
            glUniform1f(regular_program.u_time, game->time());

            glUniform1f(regular_program.u_time, game->time());

            circle_vao.draw();
        }
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
