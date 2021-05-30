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
    // Game Input
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

            case SDLK_e: {
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

        case SDL_MOUSEBUTTONDOWN: {
#ifndef SOMETHING_RELEASE
            if (editor) {
                // TODO(#98): the level editor does not allow to "draw" the tiles by dragging the mouse cursor
                switch (editor_tool) {
                case Editor_Tool::Tiles: {
                    Tile *tile = tile_grid.get_tile(World_Coord(mouse_world));
                    if (tile != NULL) {
                        tile->wall = !tile->wall;
                    }
                }
                break;

                case Editor_Tool::Enemies: {
                    spawn_enemy(mouse_world);
                }
                break;

                case Editor_Tool::Count:
                default:
                    aids::unreachable();
                }
            } else {
                player.shoot(this);
            }
#else
            player.shoot(this);
#endif
        }
        break;
        }
    }

    // Editor Input
#ifndef SOMETHING_RELEASE
    {
        switch (event->type) {
        case SDL_KEYDOWN: {
            switch (event->key.keysym.sym) {
            case SDLK_q: {
                editor = !editor;
            }
            break;

            case SDLK_z: {
                editor_tool = Editor_Tool::Tiles;
            }
            break;

            case SDLK_x: {
                editor_tool = Editor_Tool::Enemies;
            }
            break;
            }
        }
        break;
        }
    }
#endif // SOMETHING_RELEASE
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
            // Tools Panel
            {
                const auto screen_size = V2(SCREEN_WIDTH, SCREEN_HEIGHT).cast_to<float>();
                const auto tools_panel_pos =
                    screen_size * V2(-0.5f) + V2(TOOLS_PANEL_PADDING);

                // TODO(#100): editor tools panel does not allow to select tools by clicking on their icons
                // TODO(#101): editor tools panel buttons don't have any icons on them

                for (size_t i = 0; i < static_cast<size_t>(Editor_Tool::Count); ++i) {
                    const auto tool = static_cast<Editor_Tool>(i);
                    const auto tool_pos =
                        tools_panel_pos
                        + V2(i, static_cast<size_t>(0)).cast_to<float>()
                        * V2(TOOL_BUTTON_SIZE + TOOLS_PANEL_PADDING);
                    renderer->fill_aabb(
                        AABB(tool_pos, V2(TOOL_BUTTON_SIZE)),
                        tool == editor_tool ? editor_tool_color(tool) : RGBA(1.0f),
                        atlas.get_uv({0}),
                        SCREEN_PROGRAM_ASSET);
                }
            }

            // Cursor
            {
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
        }

        // Debug Label
        {
            // TODO(#102): vars.conf does not support string type

            const char *debug_text = "debug";
            const auto debug_size = font.text_size(debug_text, DEBUG_TEXT_SCALE);
            const auto screen_size = V2(SCREEN_WIDTH, SCREEN_HEIGHT).cast_to<float>();
            const auto debug_position =
                screen_size * V2(0.5f, -0.5f) -
                debug_size * V2(1.0f, 0.0f) +
                V2(DEBUG_TEXT_PADDING) * V2(-1.0f, 1.0f);

            font.render_text(
                renderer,
                debug_text,
                debug_position,
                DEBUG_TEXT_SCALE,
                DEBUG_TEXT_COLOR);
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

#ifndef SOMETHING_RELEASE
RGBA Game::editor_tool_color(Editor_Tool tool) const
{
    switch(tool) {
    case Editor_Tool::Tiles:
        return RGBA::GREEN();
    case Editor_Tool::Enemies:
        return RGBA::RED();
    case Editor_Tool::Count:
    default:
        aids::unreachable("Game::editor_tool_color()");
    }
}
#endif // SOMETHING_RELEASE
