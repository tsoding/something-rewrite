#include "./something_player.hpp"

const float PLAYER_WIDTH = 100.0f;
const float PLAYER_HEIGHT = 100.0f;
const RGBA PLAYER_COLOR = RGBA::RED();
const float PLAYER_SPEED = 500.0f;

void Player::render(const Game *game, Renderer *renderer) const
{
    auto uv = game->atlas.uvs.data[ATLAS_INDEX].flip_vertically();
    if (direction == Direction::Left) {
        uv = uv.flip_horizontally();
    }

    renderer->fill_rect(
        AABB(pos, V2(PLAYER_WIDTH, PLAYER_HEIGHT)),
        RGBA(1.0f),
        uv);
}

void Player::explode(Poof &poof, const Atlas &atlas)
{
    Triangle<GLfloat> lower, upper;
    {
        auto aabb = AABB(pos, V2(100.0f, 80.0f));
        aabb.split_into_triangles(&lower, &upper);
    }

    Triangle<GLfloat> lower_uv, upper_uv;
    {
        auto uv = atlas.uvs.data[ATLAS_INDEX].flip_vertically();
        if (direction == Direction::Left) {
            uv = uv.flip_horizontally();
        }

        uv.split_into_triangles(&lower_uv, &upper_uv);
    }

    {
        int level = 2;
        explode_triangle(poof, lower, RGBA::WHITE(), lower_uv, level);
        explode_triangle(poof, upper, RGBA::WHITE(), upper_uv, level);
    }

    pos += direction_to_v2(direction) * TELEPORTATION_DISTANCE;
}

void Player::update(Game *game, Seconds dt)
{
    const V2<float> ps[] = {
        V2(1.0f, 1.0f),
        V2(0.0f, 1.0f),
        V2(1.0f, 0.0f),
        V2(0.0f, 0.0f)
    };
    constexpr size_t ps_count = sizeof(ps) / sizeof(ps[0]);

    for (size_t i = 0; i < ps_count; ++i) {
        const auto new_vel = vel * ps[i];
        const auto new_pos = pos + new_vel * dt;
        if (!game->tile_grid.is_there_any_walls_in_region(World_Region(AABB(new_pos, V2(PLAYER_WIDTH, PLAYER_HEIGHT))))) {
            pos = new_pos;
            vel = new_vel;
            return;
        }
    }
}

void Player::jump()
{
    const float JUMP_VELOCITY = 500.0f;
    vel.y = JUMP_VELOCITY;
}

void Player::move(Direction direction)
{
    switch(direction) {
    case Direction::Left:
        vel.x = -PLAYER_SPEED;
        this->direction = direction;
        break;
    case Direction::Right:
        vel.x = PLAYER_SPEED;
        this->direction = direction;
        break;
    default:
        unreachable("Player::move()");
    }
}

void Player::stop()
{
    vel.x = 0.0f;
}

void Player::shoot(Game *game)
{
    game->projectiles.push(
        pos + V2(PLAYER_WIDTH, PLAYER_HEIGHT) * 0.5f,
        direction_to_v2(direction) * PROJECTILE_VELOCITY);
}
