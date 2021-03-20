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
}

void Game::render(Renderer *renderer) const
{
    renderer->fill_rect(
        AABB(V2(-1.0f, -1.0f), V2(2.0f, 2.0f)),
        RGBA(0.5f, 0.5f, 0.5f, 1.0f),
        0);
    player.render(this, renderer);
}
