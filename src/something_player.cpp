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

size_t longest_side(Triangle<float> tri)
{
    size_t res = 0;
    for (size_t i = 1; i < TRIANGLE_VERT_COUNT; ++i) {
        float a = length(tri.vs[(res + 1) % TRIANGLE_VERT_COUNT] - tri.vs[res]);
        float b = length(tri.vs[(i + 1) % TRIANGLE_VERT_COUNT] - tri.vs[i]);
        if (b > a) {
            res = i;
        }
    }
    return res;
}

void explode_triangle(Poof &poof, Triangle<float> vert, Triangle<float> uv,
                      int level)
{
    Triangle<GLfloat> a0, a1;
    Triangle<GLfloat> uv0, uv1;

    {
        int side = longest_side(vert);
        const float margin = 0.4f;
        float f = aids::clamp(random01(), margin, 1.0f - margin);

        split_triangle(vert, side, f, &a0, &a1);
        split_triangle(uv, side, f, &uv0, &uv1);
    }

    if (level <= 0) {
        poof.push(a0, RGBA(), uv0);
        poof.push(a1, RGBA(), uv1);
    } else {
        explode_triangle(poof, a0, uv0, level - 1);
        explode_triangle(poof, a1, uv1, level - 1);
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
        int level = 3;
        explode_triangle(poof, lower, lower_uv, level);
        explode_triangle(poof, upper, upper_uv, level);
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
