#ifndef SOMETHING_PLAYER_HPP_
#define SOMETHING_PLAYER_HPP_

#include "./something_geo.hpp"
#include "./something_triangle_vao.hpp"
#include "./something_poof.hpp"
#include "./something_tile_grid.hpp"
#include "./something_anim.hpp"
#include "./something_aabb_body.hpp"

const RGBA PLAYER_COLOR = RGBA::RED();
const float PLAYER_SPEED = 1000.0f;
const float PLAYER_GUN_SIZE = 20.0f;

struct Game;

// TODO(#64): Player does not have any health
// TODO(#65): Player can't collect the items to heal themselves
struct Player {
    // static constexpr size_t ATLAS_INDEX = 6;
    // static constexpr size_t ATLAS_INDEX = 3;
    // static constexpr size_t ATLAS_INDEX = 7;
    static constexpr size_t ATLAS_INDEX = 10;
    static constexpr float PROJECTILE_VELOCITY = 2000.0f;
    static constexpr float TELEPORTATION_DISTANCE = 300.0f;

    enum class State {
        Dead,
        Alive
    };

    State state;

    anim::Player jump_anim_player;
    bool prepare_for_jump;

    Index<AABB_Body> body_index;
    float gun_angle;
    float stretch;
    float health;

    void render(const Game *game, Triangle_VAO *triangle_vao) const;
    void update(Game *game, Seconds dt);

    void jump();
    void move(Game *game, Direction direction);
    void stop(Game *game);
    void shoot(Game *game);
    void teleport(Game *game);
    void point_gun_at(Game *game, V2<float> target);

    static Player with_body(Index<AABB_Body> body_index);
};

#endif // SOMETHING_PLAYER_HPP_
