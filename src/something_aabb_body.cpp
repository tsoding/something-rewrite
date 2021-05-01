#include "./something_aabb_body.hpp"

void AABB_Body::update(const Game *game, Seconds dt)
{
    const float GRAVITY = 4000.0f;
    vel.y -= GRAVITY * dt;

    const V2<float> ps[] = {
        V2(1.0f, 1.0f),
        V2(0.0f, 1.0f),
        V2(1.0f, 0.0f),
    };
    constexpr size_t ps_count = sizeof(ps) / sizeof(ps[0]);

    const size_t MAX_ATTEMPTS = 8;

    for (size_t attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) {
        for (size_t i = 0; i < ps_count; ++i) {
            const auto new_vel = vel * ps[i];
            const auto new_hitbox = AABB(hitbox.pos + new_vel * dt,
                                         hitbox.size);
            if (!game->tile_grid.are_there_any_walls_in_region(World_Region(new_hitbox))) {
                hitbox = new_hitbox;
                vel = new_vel;
                return;
            }
        }
        vel *= 0.5f;
    }
}
