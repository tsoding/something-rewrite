#include "./something_player.hpp"

const float PLAYER_WIDTH = 100.0f;
const float PLAYER_HEIGHT = 80.0f;
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
            AABB(pos, V2(PLAYER_WIDTH, PLAYER_HEIGHT)),
            RGBA(1.0f),
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
    Triangle<GLfloat> child_vert[4];
    Triangle<GLfloat> child_uv[4];
    size_t n = (rand() % 2 + 1) * 2;

    switch (n) {
    case 2: {
        size_t side = longest_side(vert);
        const float margin = 0.4f;
        float f = aids::clamp(random01(), margin, 1.0f - margin);

        split_triangle(vert, side, f, child_vert);
        split_triangle(uv, side, f, child_uv);
    }
    break;
    case 4: {
        const float fs[TRIANGLE_VERT_COUNT] = {0.5f, 0.5f, 0.5f};

        split_triangle(vert, fs, child_vert);
        split_triangle(uv, fs, child_uv);
    }
    break;
    default: {
        unreachable("explode_triangle()");
    }
    }

    if (level <= 0) {
        for (size_t i = 0; i < n; ++i) {
            poof.push(child_vert[i], RGBA(1.0f), child_uv[i]);
        }
    } else {
        for (size_t i = 0; i < n; ++i) {
            explode_triangle(poof, child_vert[i], child_uv[i], level - 1);
        }
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
        int level = 2;
        explode_triangle(poof, lower, lower_uv, level);
        explode_triangle(poof, upper, upper_uv, level);
    }
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
