#include "./something_player.hpp"

const float PLAYER_SIZE = 50.0f;
const RGBA PLAYER_COLOR = RGBA::RED;
const float PLAYER_SPEED = 500.0f;

void Player::render(const Game *game, Renderer *renderer) const
{
    if (!hidden) {
        auto uv = game->atlas.uvs.data[ATLAS_INDEX].flip_vertically();
        if (direction == Direction::Left) {
            uv = uv.flip_horizontally();
        }

        renderer->fill_rect(
            AABB(pos, V2(100.0f, 80.0f)),
            RGBA(),
            uv);
    }
}

void Player::explode(Poof &poof, const Atlas &atlas)
{
    hidden = true;
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
        Triangle<GLfloat> a0, a1;
        Triangle<GLfloat> uv0, uv1;

        split_triangle(lower, 0, 0.5f, &a0, &a1);
        split_triangle(lower_uv, 0, 0.5f, &uv0, &uv1);

        poof.push(a0, RGBA(), uv0);
        poof.push(a1, RGBA(), uv1);
    }

    {
        Triangle<GLfloat> a0, a1;
        Triangle<GLfloat> uv0, uv1;

        split_triangle(upper, 0, 0.5f, &a0, &a1);
        split_triangle(upper_uv, 0, 0.5f, &uv0, &uv1);

        poof.push(a0, RGBA(), uv0);
        poof.push(a1, RGBA(), uv1);
    }
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
        this->direction = direction;
        hidden = false;
        break;
    case Direction::Right:
        vel.x = PLAYER_SPEED;
        this->direction = direction;
        hidden = false;
        break;
    default:
        unreachable("Player::move()");
    }
}

void Player::stop()
{
    vel.x = 0.0f;
}
