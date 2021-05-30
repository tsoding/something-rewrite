#include "./something_game.hpp"

const RGBA FAILED_BACKGROUND_COLOR = RGBA::from_abgr32(0xAA1818FF);

void Game::init(SDL_Window *window)
{
    const V2<float> start = V2(1947.0f, 1818.5f);

    // Player
    {
        this->player = Player::with_body(
                           this->allocate_aabb_body(
                               AABB(start, V2(PLAYER_WIDTH, PLAYER_HEIGHT))));
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
    this->font = Font::from_file("./assets/textures/charmap-cellphone_white.png");

    this->keyboard = SDL_GetKeyboardState(NULL);
    this->window = window;

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
    return (p - V2(SCREEN_WIDTH, SCREEN_HEIGHT).cast_to<float>() * V2(0.5f)) * V2(camera.zoom) + camera.pos;
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

#ifndef SOMETHING_RELEASE
        case SDLK_F3: {
            editor = !editor;
        }
        break;
#endif
        }
    }
    break;

    case SDL_MOUSEMOTION: {
        mouse_window = V2(event->motion.x, event->motion.y);
    }
    break;

    case SDL_MOUSEBUTTONDOWN: {
#ifndef SOMETHING_RELEASE
        if (editor) {
            Tile *tile = tile_grid.get_tile(World_Coord(mouse_world));
            if (tile != NULL) {
                tile->wall = !tile->wall;
            }
        } else {
            player.shoot(this);
        }
#else
        player.shoot(this);
#endif
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
        camera.vel = (target_pos - camera.pos) * V2(4.0f);
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

void Game::render(Renderer *renderer) const
{
    renderer->clear();
    {
        tile_grid.render(this, renderer);
        player.render(this, renderer);
        poof.render(renderer);
        projectiles.render(renderer);
        for (size_t i = 0; i < enemies_size; ++i) {
            enemies[i].render(this, renderer);
        }
        for (size_t i = 0; i < items_size; ++i) {
            items[i].render(this, renderer);
        }
        particles.render(renderer);

#ifndef SOMETHING_RELEASE
        if (editor) {
            const auto mouse_screen =
                window_to_viewport(window, mouse_window) -
                V2(SCREEN_WIDTH, SCREEN_HEIGHT).cast_to<float>() * V2(0.5f);
            const auto cursor_texture_uv = atlas.get_uv({static_cast<size_t>(MOUSE_CURSOR_TEXTURE)}).flip_vertically();
            const auto cursor_size = atlas.get_size({static_cast<size_t>(MOUSE_CURSOR_TEXTURE)}, MOUSE_CURSOR_SIZE);

            renderer->fill_aabb(
                AABB(mouse_screen - cursor_size * V2(0.0f, 1.0f), cursor_size),
                MOUSE_CURSOR_COLOR,
                cursor_texture_uv,
                SCREEN_PROGRAM_ASSET);
        }
#endif
    }
    renderer->draw(this);
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

Index<AABB_Body> Game::allocate_aabb_body(AABB<float> hitbox)
{
    assert(aabb_bodies_size < AABB_BODIES_CAPACITY);
    Index<AABB_Body> result = {aabb_bodies_size};
    aabb_bodies[aabb_bodies_size] = AABB_Body::from_hitbox(hitbox);
    aabb_bodies_size += 1;
    return result;
}

void Game::spawn_enemy(V2<float> pos)
{
    if (enemies_size < ENEMIES_CAPACITY) {
        enemies[enemies_size].state = Enemy::Alive;
        enemies[enemies_size].body_index =
            allocate_aabb_body(
                AABB(pos, V2(100.0f * 2.0f, 80.0f * 2.0f)));
        enemies[enemies_size].direction = 1.0f;

        enemies_size += 1;
    }
}
