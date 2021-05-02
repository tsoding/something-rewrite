#include "./something_player.hpp"

void Player::render(const Game *game, Triangle_VAO *triangle_vao) const
{
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

        const auto &body = game->get_aabb_body(body_index);
        const auto player_hitbox = aabb_stretch(body.hitbox, stretch);
        triangle_vao->fill_aabb(player_hitbox, RGBA(1.0f), uv);
    }

    // Player gun
    {
        const auto &body = game->get_aabb_body(body_index);
        const auto pos = body.center();
        const auto gun_pos = pos + polar_v2(gun_angle, max(PLAYER_WIDTH, PLAYER_HEIGHT));
        const auto gun = equilateral_triangle(gun_pos, PLAYER_GUN_SIZE, gun_angle);
        triangle_vao->fill_triangle(gun, RGBA::RED(), {});
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
        explode_triangle(game->poof, lower, RGBA::WHITE(), lower_uv, level);
        explode_triangle(game->poof, upper, RGBA::WHITE(), upper_uv, level);
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
