#include "./something_player.hpp"

const float PLAYER_WIDTH = 100.0f;
const float PLAYER_HEIGHT = 100.0f;
const RGBA PLAYER_COLOR = RGBA::RED();
const float PLAYER_SPEED = 500.0f;
const float PLAYER_GUN_SIZE = 20.0f;

static AABB<float> player_hitbox(V2<float> pos)
{
    const auto size = V2(PLAYER_WIDTH, PLAYER_HEIGHT);
    return AABB(pos - size * 0.5f, size);
}

void Player::render(const Game *game, Renderer *renderer) const
{
    // Player body
    {
        auto uv = game->atlas.uvs.data[ATLAS_INDEX].flip_vertically();
        if (direction == Direction::Left) {
            uv = uv.flip_horizontally();
        }

        renderer->fill_rect(player_hitbox(pos), RGBA(1.0f), uv);
    }

    // Payer gun
    {
        const auto gun_pos = pos + polar_v2(gun_angle, max(PLAYER_WIDTH, PLAYER_HEIGHT));
        const auto gun = equilateral_triangle(gun_pos, PLAYER_GUN_SIZE, gun_angle);
        renderer->fill_triangle(gun, RGBA::RED(), {});
    }
}

void Player::teleport(Game *game)
{
    Triangle<GLfloat> lower, upper;
    {
        auto aabb = player_hitbox(pos);
        aabb.split_into_triangles(&lower, &upper);
    }

    Triangle<GLfloat> lower_uv, upper_uv;
    {
        auto uv = game->atlas.uvs.data[ATLAS_INDEX].flip_vertically();
        if (direction == Direction::Left) {
            uv = uv.flip_horizontally();
        }

        uv.split_into_triangles(&lower_uv, &upper_uv);
    }

    {
        int level = 2;
        explode_triangle(game->poof, lower, RGBA::WHITE(), lower_uv, level);
        explode_triangle(game->poof, upper, RGBA::WHITE(), upper_uv, level);
    }

    pos += polar_v2(gun_angle, TELEPORTATION_DISTANCE);
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
        if (!game->tile_grid.is_there_any_walls_in_region(World_Region(player_hitbox(new_pos)))) {
            pos = new_pos;
            vel = new_vel;
            return;
        }
    }
}

void Player::jump()
{
    const float JUMP_VELOCITY = 1000.0f;
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
    const auto gun_pos = pos + polar_v2(gun_angle, max(PLAYER_WIDTH, PLAYER_HEIGHT));
    game->projectiles.push(
        gun_pos,
        polar_v2(gun_angle, PROJECTILE_VELOCITY));
}

void Player::point_gun_at(V2<float> target)
{
    gun_angle = angle_v2(target - pos);
}
