#include "./something_game.hpp"
#include "./something_sdl.hpp"
#include "./something_config.hpp"

const RGBA BACKGROUND_COLOR = RGBA::from_abgr32(0x181818FF);

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
    player.render(this, renderer);
}
