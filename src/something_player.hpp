#ifndef SOMETHING_PLAYER_HPP_
#define SOMETHING_PLAYER_HPP_

#include "./something_sdl.hpp"
#include "./something_geo.hpp"
#include "./something_renderer.hpp"

enum class Direction {
    Left,
    Right,
};

struct Game;

struct Player {
    V2<float> pos;
    V2<float> vel;

    void render(const Game *game, Renderer *renderer) const;
    void update(Game *game, Seconds dt);

    void move(Direction direction);
    void stop();
};

#endif // SOMETHING_PLAYER_HPP_
