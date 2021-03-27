#include "./something_game.hpp"

const RGBA BACKGROUND_COLOR = RGBA::from_abgr32(0x181818FF);
const RGBA FAILED_BACKGROUND_COLOR = RGBA::from_abgr32(0xAA1818FF);

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
            player.explode(poof, atlas);
        } break;
        }
    }
    break;
    }
}

void Game::update(Seconds dt)
{
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
        player.resolve_collisions(tile_grid);

        const float GROUND = -200.0f;
        const float GRAVITY = 2000.0f;
        if (player.pos.y <= GROUND) {
            player.pos.y = GROUND;
            player.vel.y = 0.0f;
        } else {
            player.vel.y -= GRAVITY * dt;
        }
    }

    // Camera
    {
        camera.update(dt);
        camera.vel = player.pos - camera.pos;
    }

    // Poof
    {
        poof.update(dt);
    }
}

void Game::render(Renderer *renderer) const
{
    tile_grid.render(this, renderer);
    player.render(this, renderer);
    poof.render(renderer);
}
