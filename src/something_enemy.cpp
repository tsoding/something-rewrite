#include "./something_enemy.hpp"

void Enemy::render(const Game *game, Triangle_VAO *triangle_vao) const
{
    const auto &body = game->get_aabb_body(body_index);
    const size_t ATLAS_INDEX = 6;
    const auto uv = game->atlas.uvs.data[ATLAS_INDEX].flip_vertically();
    triangle_vao->fill_aabb(body.hitbox, RGBA(1.0f), uv);
}

void Enemy::update(Game*, Seconds)
{
}
