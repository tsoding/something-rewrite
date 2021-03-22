#ifndef SOMETHING_PLAYER_HPP_
#define SOMETHING_PLAYER_HPP_

#include "./something_geo.hpp"
#include "./something_renderer.hpp"
#include "./something_poof.hpp"

enum class Direction {
    Right = 0,
    Left,
};

struct Game;

struct Player {
    // static constexpr size_t ATLAS_INDEX = 6;
    // static constexpr size_t ATLAS_INDEX = 3;
    static constexpr size_t ATLAS_INDEX = 7;

    V2<float> pos;
    V2<float> vel;
    Direction direction;
    bool hidden;

    void render(const Game *game, Renderer *renderer) const;
    void update(Game *game, Seconds dt);

    void jump();
    void move(Direction direction);
    void stop();
    void explode(Poof &poof, const Atlas &atlas);
};

#endif // SOMETHING_PLAYER_HPP_
