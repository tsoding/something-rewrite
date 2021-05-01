#include "./something_game.hpp"

const RGBA BACKGROUND_COLOR = RGBA::from_abgr32(0x181818FF);
const RGBA FAILED_BACKGROUND_COLOR = RGBA::from_abgr32(0xAA1818FF);

void Game::init(SDL_Window *window)
{
    const V2<float> start = V2(1947.0f, 1818.5f);

    // Player
    {
        this->player.body_index = allocate_aabb_body();
        {
            auto &player_body = get_aabb_body(this->player.body_index);
            player_body.hitbox = AABB(start, V2(PLAYER_WIDTH, PLAYER_HEIGHT));
        }
        this->player.jump_anim_player.segments = jump_anim;
        this->player.jump_anim_player.segments_count = Jump_Anim_Size;
    }

    // Enemies
    {
        spawn_enemy(start);
    }

    // Camera
    {
        this->camera.zoom = 1.0f;
        this->camera_follow_body = this->player.body_index;
    }

    this->atlas = Atlas::from_config("./assets/textures/atlas.conf", 10);
    this->keyboard = SDL_GetKeyboardState(NULL);
    this->window = window;

    // Shader Programs
    {
        regular_program = Program::load_from_shader_files(
                              "./assets/shaders/rect.vert",
                              "./assets/shaders/rect.frag");

        particle_program = Program::load_from_shader_files(
                               "./assets/shaders/circle.vert",
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

    // Items
    {
        items[items_size++] = Item::make_tea(start);
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
    return (p - V2(SCREEN_WIDTH, SCREEN_HEIGHT).cast_to<float>() * 0.5f) * camera.zoom + camera.pos;
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

        case SDLK_i: {
            println(stdout, aabb_bodies[0].hitbox);
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

    case SDL_MOUSEWHEEL: {
        const float MIN_ZOOM = 1e-3f;
        if (event->wheel.y > 0) {
            camera.zoom *= 0.75f;
        } else if (event->wheel.y < 0) {
            camera.zoom *= 1.75f;
        }

        camera.zoom = max(camera.zoom, MIN_ZOOM);
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
            player.move(this, Direction::Right);
        } else if (keyboard[SDL_SCANCODE_A]) {
            player.move(this, Direction::Left);
        } else {
            player.stop(this);
        }

        player.update(this, dt);
        player.point_gun_at(this, mouse_world);
    }

    // Camera
    {
        camera.update(dt);

        const auto target_pos = get_aabb_body(camera_follow_body).hitbox.pos;
        camera.vel = (target_pos - camera.pos) * 4.0f;
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

    // Enemies
    {
        for (size_t i = 0; i < enemies_size; ++i) {
            enemies[i].update(this, dt);
        }
    }

    // Bodies
    {
        for (size_t i = 0; i < aabb_bodies_size; ++i) {
            aabb_bodies[i].update(this, dt);
        }
    }
}

void Game::render(Triangle_VAO *triangle_vao,
                  Circle_VAO *circle_vao) const
{
    if (!regular_program.failed && !particle_program.failed) {
        // Regular things
        {
            triangle_vao->use();
            triangle_vao->clear();

            tile_grid.render(this, triangle_vao);
            player.render(this, triangle_vao);
            poof.render(triangle_vao);
            projectiles.render(triangle_vao);
            for (size_t i = 0; i < enemies_size; ++i) {
                enemies[i].render(this, triangle_vao);
            }
            for (size_t i = 0; i < items_size; ++i) {
                items[i].render(this, triangle_vao);
            }

            triangle_vao->sync_buffers();

            regular_program.use();
            glUniform2f(regular_program.u_resolution, SCREEN_WIDTH, SCREEN_HEIGHT);
            glUniform2f(regular_program.u_camera_position, camera.pos.x, camera.pos.y);
            glUniform1f(regular_program.u_camera_zoom, camera.zoom);
            glUniform1f(regular_program.u_time, time());

            triangle_vao->draw();
        }

        // Particle things
        {
            circle_vao->use();
            circle_vao->clear();
            particles.render(circle_vao);
            circle_vao->sync_buffers();

            particle_program.use();
            glUniform2f(particle_program.u_resolution, SCREEN_WIDTH, SCREEN_HEIGHT);
            glUniform2f(particle_program.u_camera_position, camera.pos.x, camera.pos.y);
            glUniform1f(particle_program.u_camera_zoom, camera.zoom);
            glUniform1f(particle_program.u_time, time());

            circle_vao->draw();
        }
    }
}

Seconds Game::time() const
{
    return static_cast<float>(SDL_GetTicks()) / 1000.0f;
}

const AABB_Body &Game::get_aabb_body(Index<AABB_Body> body_index) const
{
    assert(body_index.unwrap < AABB_BODIES_CAPACITY);
    return aabb_bodies[body_index.unwrap];
}

AABB_Body &Game::get_aabb_body(Index<AABB_Body> body_index)
{
    assert(body_index.unwrap < AABB_BODIES_CAPACITY);
    return aabb_bodies[body_index.unwrap];
}

Index<AABB_Body> Game::allocate_aabb_body()
{
    assert(aabb_bodies_size < AABB_BODIES_CAPACITY);
    return {aabb_bodies_size++};
}

void Game::spawn_enemy(V2<float> pos)
{
    if (enemies_size < ENEMIES_CAPACITY) {
        enemies[enemies_size].state = Enemy::Alive;
        enemies[enemies_size].body_index = allocate_aabb_body();
        enemies[enemies_size].direction = 1.0f;
        get_aabb_body(enemies[enemies_size].body_index).hitbox =
            AABB(pos, V2(100.0f * 2.0f, 80.0f * 2.0f));
        enemies_size += 1;
    }
}
