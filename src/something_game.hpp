#ifndef SOMETHING_GAME_HPP_
#define SOMETHING_GAME_HPP_

#include "./something_player.hpp"
#include "./something_renderer.hpp"
#include "./something_camera.hpp"
#include "./something_tile_grid.hpp"
#include "./something_poof.hpp"

struct Game
{
    bool quit;

    const Uint8 *keyboard;

    Atlas atlas;
    Player player;
    Camera camera;
    Tile_Grid tile_grid;
    Poof poof;

    void init();
    void handle_event(const SDL_Event *event);
    void update(Seconds dt);
    void render(Renderer *renderer) const;
};

#endif  // SOMETHING_GAME_HPP_
