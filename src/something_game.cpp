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
    }
}

void Game::update(Seconds dt)
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
    const float GRAVITY = 1000.0f;
    if (player.pos.y <= GROUND) {
        player.pos.y = GROUND;
        player.vel.y = 0.0f;
    } else {
        player.vel.y -= GRAVITY * dt;
    }
}

void Game::render(Renderer *renderer) const
{
    player.render(this, renderer);
}
