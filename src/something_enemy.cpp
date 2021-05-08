#include "./something_enemy.hpp"
#include "./something_projectiles.hpp"

void Enemy::render(const Game *game, Renderer *renderer) const
{
    if (state == Alive) {
        const auto &body = game->get_aabb_body(body_index);
        const size_t ATLAS_INDEX = 6;
        auto uv = game->atlas.uvs.data[ATLAS_INDEX].flip_vertically();

        if (dummy_idle_direction(game) < 0.0) {
            uv = uv.flip_horizontally();
        }

        renderer->fill_aabb(body.hitbox, RGBA(1.0f), uv);
    }
}

void Enemy::update(Game *game, Seconds)
{
    if (state == Alive) {
        auto &body = game->get_aabb_body(body_index);
        for (size_t i = 0; i < Projectiles::CAPACITY; ++i) {
            if (game->projectiles.states[i] == Projectiles::State::Aliv && body.hitbox.contains(game->projectiles.positions[i])) {
                game->projectiles.states[i] = Projectiles::State::Ded;
                kill(game);
                game->spawn_enemy(V2(1947.0f, 1818.5f));
            }
        }

        body.vel.x = dummy_idle_direction(game) * ENEMY_SPEED;
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
            explode_triangle(game->poof, lower, RGBA::WHITE(), lower_uv, level);
            explode_triangle(game->poof, upper, RGBA::WHITE(), upper_uv, level);
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
