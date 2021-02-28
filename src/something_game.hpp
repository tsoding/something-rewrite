#ifndef SOMETHING_GAME_HPP_
#define SOMETHING_GAME_HPP_

#include "./something_player.hpp"
#include "./something_renderer.hpp"

struct Game
{
    bool quit;

    const Uint8 *keyboard;

    Player player;

    void handle_event(const SDL_Event *event);
    void update(Seconds dt);
    void render(Renderer *renderer) const;
};

#endif  // SOMETHING_GAME_HPP_
