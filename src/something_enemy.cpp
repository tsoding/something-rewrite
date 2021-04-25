#include "./something_enemy.hpp"
#include "./something_projectiles.hpp"

void Enemy::render(const Game *game, Triangle_VAO *triangle_vao) const
{
    if (state == Alive) {
        const auto &body = game->get_aabb_body(body_index);
        const size_t ATLAS_INDEX = 6;
        const auto uv = game->atlas.uvs.data[ATLAS_INDEX].flip_vertically();
        triangle_vao->fill_aabb(body.hitbox, RGBA(1.0f), uv);
    }
}

void Enemy::update(Game *game, Seconds)
{
    if (state == Alive) {
        const auto &body = game->get_aabb_body(body_index);
        for (size_t i = 0; i < Projectiles::CAPACITY; ++i) {
            if (game->projectiles.states[i] == Projectiles::State::Aliv && body.hitbox.contains(game->projectiles.positions[i])) {
                game->projectiles.states[i] = Projectiles::State::Ded;
                kill(game);
                game->spawn_enemy(V2(1947.0f, 1818.5f));
            }
        }
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

