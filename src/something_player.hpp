#ifndef SOMETHING_PLAYER_HPP_
#define SOMETHING_PLAYER_HPP_

#include "./something_geo.hpp"
#include "./something_renderer.hpp"
#include "./something_poof.hpp"
#include "./something_tile_grid.hpp"

enum class Direction {
    Right = 0,
    Left,
};

constexpr V2<float> direction_to_v2(Direction direction)
{
    switch (direction) {
    case Direction::Right:
        return V2(1.0f, 0.0f);
    case Direction::Left:
        return V2(-1.0f, 0.0f);
    default:
        assert(false && "Thank you Straustrup");
    }
}

struct Game;

struct Player {
    // static constexpr size_t ATLAS_INDEX = 6;
    static constexpr size_t ATLAS_INDEX = 3;
    // static constexpr size_t ATLAS_INDEX = 7;
    static constexpr float PROJECTILE_VELOCITY = 1000.0f;
    static constexpr float TELEPORTATION_DISTANCE = 300.0f;

    V2<float> pos;
    V2<float> vel;
    Direction direction;

    void render(const Game *game, Renderer *renderer) const;
    void update(Game *game, Seconds dt);

    void jump();
    void move(Direction direction);
    void stop();
    void shoot(Game *game);
    void explode(Poof &poof, const Atlas &atlas);
};

#endif // SOMETHING_PLAYER_HPP_
