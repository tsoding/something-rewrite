#include "./something_game.hpp"

const RGBA BACKGROUND_COLOR = RGBA::from_abgr32(0x181818FF);
const RGBA FAILED_BACKGROUND_COLOR = RGBA::from_abgr32(0xAA1818FF);

void Game::init(SDL_Window *window)
{
    this->camera.z = Camera::DISTANCE;

    this->player.pos = V2(-100.0f, 0.0f);

    this->atlas = Atlas::from_config("./assets/textures/atlas.conf", 10);

    this->keyboard = SDL_GetKeyboardState(NULL);

    this->window = window;

    // Shader Programs
    {
        regular_program = Program::load_from_shader_files(
                              "./assets/shaders/rect.vert",
                              "./assets/shaders/rect.frag");

        particle_program = Program::load_from_shader_files(
                               "./assets/shaders/rect.vert",
                               "./assets/shaders/particle.frag");

    }

    // Testing tiles
    {
        for (int i = 0; i < 10; ++i) {
            auto tile = this->tile_grid.get_tile(Tile_Coord(V2(i)));
            if (tile) {
                tile->wall = true;
            }
        }

        for (int i = 10; i < 20; ++i) {
            auto tile = this->tile_grid.get_tile(Tile_Coord(V2(i, 10)));
            if (tile) {
                tile->wall = true;
            }
        }

        for (int i = 10; i < 20; ++i) {
            auto tile = this->tile_grid.get_tile(Tile_Coord(V2(20, i)));
            if (tile) {
                tile->wall = true;
            }
        }
    }

    // Particles
    {
        const float MAX_PARTICLE_REGION = 500.0f;
        for (size_t i = 0; i < Particles::CAPACITY; ++i) {
            particles.push(V2(lerp(0.0f, MAX_PARTICLE_REGION, random01()),
                              lerp(0.0f, MAX_PARTICLE_REGION, random01())));
        }
    }
}

AABB<float> compute_gl_viewport(int w, int h);

V2<float> window_to_viewport(SDL_Window *window, V2<Sint32> p)
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    const auto viewport = compute_gl_viewport(w, h);

    p.y = h - p.y;

    const float mouse_x =
        lerp(0.0f, static_cast<float>(SCREEN_WIDTH),
             inv_lerp(
                 viewport.pos.x,
                 viewport.pos.x + viewport.size.x,
                 static_cast<float>(p.x)));

    const float mouse_y =
        lerp(0.0f, static_cast<float>(SCREEN_HEIGHT),
             inv_lerp(
                 viewport.pos.y,
                 viewport.pos.y + viewport.size.y,
                 static_cast<float>(p.y)));

    return V2(mouse_x, mouse_y);
}

V2<float> viewport_to_world(const Camera &camera, V2<float> p)
{
    // TODO(#38): viewport_to_world() does not apply camera scaling
    return p - V2(SCREEN_WIDTH, SCREEN_HEIGHT).cast_to<float>() * 0.5f + camera.pos;
}

void Game::handle_event(const SDL_Event *event)
{
    switch (event->type) {
    case SDL_QUIT: {
        quit = true;
    }
    break;

    case SDL_KEYDOWN: {
        switch (event->key.keysym.sym) {
        case SDLK_SPACE: {
            player.jump();
        }
        break;

        case SDLK_q: {
            player.teleport(this);
        }
        break;
        }
    }
    break;

    case SDL_MOUSEMOTION: {
        mouse_window = V2(event->motion.x, event->motion.y);
    }
    break;

    case SDL_MOUSEBUTTONDOWN: {
        player.shoot(this);
    }
    break;
    }
}

void Game::update(Seconds dt)
{
    // Mouse
    {
        mouse_world =
            viewport_to_world(
                camera,
                window_to_viewport(
                    window,
                    mouse_window));
    }

    // Player
    {
        if (keyboard[SDL_SCANCODE_D]) {
            player.move(Direction::Right);
        } else if (keyboard[SDL_SCANCODE_A]) {
            player.move(Direction::Left);
        } else {
            player.stop();
        }

        player.update(this, dt);

        const float GROUND = -200.0f;
        const float GRAVITY = 3000.0f;
        if (player.pos.y <= GROUND) {
            player.pos.y = GROUND;
            player.vel.y = 0.0f;
        } else {
            player.vel.y -= GRAVITY * dt;
        }

        player.point_gun_at(mouse_world);
    }

    // Camera
    {
        camera.update(dt);
        camera.vel = (player.pos - camera.pos) * 4.0f;
    }

    // Poof
    {
        poof.update(dt);
    }

    // Projectiles
    {
        projectiles.update(this, dt);
    }

    // Particles
    {
        particles.update(dt);
    }
}

void Game::render(Renderer *renderer) const
{
    if (!regular_program.failed && !particle_program.failed) {
        // Regular things
        {
            tile_grid.render(this, renderer);
            player.render(this, renderer);
            poof.render(renderer);
            projectiles.render(renderer);

            const float MOUSE_PROBE_SIZE = 50.0f;
            renderer->fill_rect(AABB(mouse_world - V2(MOUSE_PROBE_SIZE * 0.5f),
                                     V2(MOUSE_PROBE_SIZE)),
                                RGBA::RED(),
                                atlas.uvs.data[0]);

            regular_program.use();
            glUniform2f(regular_program.u_resolution, SCREEN_WIDTH, SCREEN_HEIGHT);
            glUniform2f(regular_program.u_camera_position, camera.pos.x, camera.pos.y);
            glUniform1f(regular_program.u_camera_scale, camera.z / Camera::DISTANCE);
            glUniform1f(regular_program.u_time, static_cast<float>(SDL_GetTicks()) / 1000.0f);
            renderer->draw();
            renderer->clear();
        }

        // Particle things
        {
            particles.render(renderer);
            particle_program.use();
            glUniform2f(particle_program.u_resolution, SCREEN_WIDTH, SCREEN_HEIGHT);
            glUniform2f(particle_program.u_camera_position, camera.pos.x, camera.pos.y);
            glUniform1f(particle_program.u_camera_scale, camera.z / Camera::DISTANCE);
            glUniform1f(particle_program.u_time, static_cast<float>(SDL_GetTicks()) / 1000.0f);
            renderer->draw();
            renderer->clear();
        }
    }
}
