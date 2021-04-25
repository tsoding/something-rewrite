#ifndef SOMETHING_PLAYER_HPP_
#define SOMETHING_PLAYER_HPP_

#include "./something_geo.hpp"
#include "./something_triangle_vao.hpp"
#include "./something_poof.hpp"
#include "./something_tile_grid.hpp"
#include "./something_anim.hpp"
#include "./something_aabb_body.hpp"

const float PLAYER_WIDTH = 100.0f;
const float PLAYER_HEIGHT = 100.0f;
const RGBA PLAYER_COLOR = RGBA::RED();
const float PLAYER_SPEED = 1000.0f;
const float PLAYER_GUN_SIZE = 20.0f;

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
        return V2(0.0f);
    }
}

struct Game;

enum Jump_Anim: size_t {
    Jump_Anim_Prepare = 0,
    Jump_Anim_Attack,
    Jump_Anim_Recover,
    Jump_Anim_Size
};

const float Jump_Anim_Intensity = 0.20f;

static const anim::Segment jump_anim[Jump_Anim_Size] = {
    {1.0f,                       1.0f - Jump_Anim_Intensity, 0.05f, sqrtf},
    {1.0f - Jump_Anim_Intensity, 1.0f + Jump_Anim_Intensity, 0.1f,  squaref},
    {1.0f + Jump_Anim_Intensity, 1.0f,                       0.25f,  sqrtf}
};

struct Player {
    // static constexpr size_t ATLAS_INDEX = 6;
    // static constexpr size_t ATLAS_INDEX = 3;
    // static constexpr size_t ATLAS_INDEX = 7;
    static constexpr size_t ATLAS_INDEX = 10;
    static constexpr float PROJECTILE_VELOCITY = 2000.0f;
    static constexpr float TELEPORTATION_DISTANCE = 300.0f;

    anim::Player jump_anim_player;
    bool prepare_for_jump;


    Index<AABB_Body> body_index;
    float gun_angle;
    float stretch;

    void render(const Game *game, Triangle_VAO *triangle_vao) const;
    void update(Game *game, Seconds dt);

    void jump();
    void move(Game *game, Direction direction);
    void stop(Game *game);
    void shoot(Game *game);
    void teleport(Game *game);
    void point_gun_at(Game *game, V2<float> target);
};

#endif // SOMETHING_PLAYER_HPP_
