#include "./something_aabb_body.hpp"

void AABB_Body::update(const Game *game, Seconds dt)
{
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
            const auto new_hitbox = AABB(hitbox.pos + new_vel * V2(dt),
                                         hitbox.size);
            if (!game->tile_grid.are_there_any_walls_in_region(World_Region(new_hitbox))) {
                hitbox = new_hitbox;
                vel = new_vel;
                return;
            }
        }
        vel *= V2(0.5f);
    }
}

void AABB_Body::move(Direction direction, float speed)
{
    switch(direction) {
    case Direction::Left:
        vel.x = -speed;
        break;
    case Direction::Right:
        vel.x = speed;
        break;
    default:
        unreachable("AABB_Body::move()");
    }
}

void AABB_Body::stop()
{
    vel.x = 0.0f;
}

AABB_Body AABB_Body::from_hitbox(AABB<float> hitbox)
{
    AABB_Body self = {};
    self.vel = V2(0.0f);
    self.hitbox = hitbox;
    return self;
}
