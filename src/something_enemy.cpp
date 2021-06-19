#include "./something_enemy.hpp"
#include "./something_projectiles.hpp"

void Enemy::update(Game *game, Seconds)
{
    if (state == Alive) {
        auto &body = game->get_aabb_body(body_index);
        for (size_t i = 0; i < Projectiles::CAPACITY; ++i) {
            if (game->projectiles.states[i] == Projectiles::State::Aliv && body.hitbox.contains(game->projectiles.positions[i])) {
                game->projectiles.states[i] = Projectiles::State::Ded;
                kill(game);
            }
        }

        body.vel.x = dummy_idle_direction(game) * ENEMY_SPEED;

        const size_t ATLAS_INDEX = 6;
        auto uv = game->atlas.uvs.data[ATLAS_INDEX].flip_vertically();

        if (dummy_idle_direction(game) < 0.0) {
            uv = uv.flip_horizontally();
        }

        game->renderer.fill_aabb(body.hitbox, RGBA(1.0f), uv, REGULAR_PROGRAM_ASSET);
    }
}

void Enemy::kill(Game *game)
{
    // Explode
    {
        const size_t ATLAS_INDEX = 6;
        auto &body = game->get_aabb_body(body_index);

        Triangle<GLfloat> lower, upper;
        body.hitbox.split_into_triangles(&lower, &upper);

        Triangle<GLfloat> lower_uv, upper_uv;
        {
            auto uv = game->atlas.uvs.data[ATLAS_INDEX].flip_vertically();
            uv.split_into_triangles(&lower_uv, &upper_uv);
        }

        {
            int level = 2;
            game->poof.explode_triangle(lower, RGBA::WHITE(), lower_uv, level);
            game->poof.explode_triangle(upper, RGBA::WHITE(), upper_uv, level);
        }
    }

    state = Ded;
}


float Enemy::dummy_idle_direction(const Game *game) const
{
    const Seconds time = game->time();
    const Seconds period = 1.0f;
    const float direction = static_cast<float>(2 * (static_cast<int>(floorf(time / period)) & 1) - 1);
    return direction;
}
