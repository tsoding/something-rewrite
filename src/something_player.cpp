#include "./something_player.hpp"

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

void Player::render(const Game *game, Renderer *renderer) const
{
    const auto &body = game->get_aabb_body(body_index);

    // Player body
    {
        auto uv = game->atlas.uvs.data[ATLAS_INDEX].flip_vertically();

        {
            const float pi = static_cast<float>(M_PI);
            const float a = fmodulof(gun_angle, 2.0f * pi) / pi;
            if (0.5f <= a && a <= 1.5f) {
                uv = uv.flip_horizontally();
            }
        }

        const auto player_hitbox = aabb_stretch(body.hitbox, stretch);
        renderer->fill_aabb(player_hitbox, RGBA(1.0f), uv, REGULAR_PROGRAM_ASSET);
    }

    // Player gun
    {
        const auto pos = body.center();
        const auto gun_pos = pos + polar_v2(gun_angle, max(PLAYER_WIDTH, PLAYER_HEIGHT));
        const auto gun = equilateral_triangle(gun_pos, PLAYER_GUN_SIZE, gun_angle);
        renderer->fill_triangle(gun, RGBA::RED(), {}, REGULAR_PROGRAM_ASSET);
    }

    // Player health
    {
        const float health_p = health / PLAYER_MAX_HEALTH;

        const V2 offset =
            V2(HEALTH_BAR_WIDTH, body.hitbox.size.y) * V2(-0.5f, 0.5f) +
            V2(0.0f, HEALTH_BAR_PADDING);
        renderer->fill_aabb(
            AABB(body.center() + offset, V2(HEALTH_BAR_WIDTH * health_p, HEALTH_BAR_HEIGHT)),
            HEALTH_BAR_COLOR,
            game->atlas.get_uv({0}),
            REGULAR_PROGRAM_ASSET);
    }
}

void Player::teleport(Game *game)
{
    auto &body = game->get_aabb_body(body_index);

    Triangle<GLfloat> lower, upper;
    body.hitbox.split_into_triangles(&lower, &upper);

    Triangle<GLfloat> lower_uv, upper_uv;
    {
        auto uv = game->atlas.uvs.data[ATLAS_INDEX].flip_vertically();
        {
            const float pi = static_cast<float>(M_PI);
            const float a = fmodulof(gun_angle, 2.0f * pi) / pi;
            if (0.5f <= a && a <= 1.5f) {
                uv = uv.flip_horizontally();
            }
        }

        uv.split_into_triangles(&lower_uv, &upper_uv);
    }

    {
        int level = 2;
        game->poof.explode_triangle(lower, RGBA::WHITE(), lower_uv, level);
        game->poof.explode_triangle(upper, RGBA::WHITE(), upper_uv, level);
    }

    body.hitbox.pos += polar_v2(gun_angle, TELEPORTATION_DISTANCE);
}

void Player::update(Game *game, Seconds dt)
{
    auto &body = game->get_aabb_body(body_index);

    // Jump Animation Test
    {
        if (prepare_for_jump &&
                jump_anim_player.segment_current >= Jump_Anim_Attack) {
            const float JUMP_VELOCITY = 2000.0f;
            body.vel.y = JUMP_VELOCITY;
            prepare_for_jump = false;
        }
        stretch = jump_anim_player.update(dt);
    }
}

void Player::jump()
{
    jump_anim_player.reset();
    prepare_for_jump = true;
}

void Player::move(Game *game, Direction direction)
{
    game->get_aabb_body(body_index).move(direction, PLAYER_SPEED);
}

void Player::stop(Game *game)
{
    game->get_aabb_body(body_index).stop();
}

void Player::shoot(Game *game)
{
    const auto gun_pos = game->get_aabb_body(body_index).center() + polar_v2(gun_angle, max(PLAYER_WIDTH, PLAYER_HEIGHT));
    game->projectiles.push(
        gun_pos,
        polar_v2(gun_angle, PROJECTILE_VELOCITY));
}

void Player::point_gun_at(Game *game, V2<float> target)
{
    gun_angle = angle_v2(target - game->get_aabb_body(body_index).center());
}

Player Player::with_body(Index<AABB_Body> body_index)
{
    Player player = {};
    player.body_index = body_index;
    player.jump_anim_player.segments = jump_anim;
    player.jump_anim_player.segments_count = Jump_Anim_Size;
    player.health = PLAYER_MAX_HEALTH;
    return player;
}
