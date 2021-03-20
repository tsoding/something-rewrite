#include "./something_player.hpp"

const float PLAYER_SIZE = 50.0f;
const RGBA PLAYER_COLOR = RGBA::RED;
const float PLAYER_SPEED = 500.0f;

void Player::render(const Game *, Renderer *renderer) const
{
    renderer->fill_rect(
        AABB(pos, V2(100.0f, 80.0f)),
        RGBA(),
        6);
}

void Player::update(Game *, Seconds dt)
{
    pos += vel * dt;
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
        break;
    case Direction::Right:
        vel.x = PLAYER_SPEED;
        break;
    default:
        unreachable("Player::move()");
    }
}

void Player::stop()
{
    vel.x = 0.0f;
}
