#ifndef SOMETHING_AABB_BODY_HPP_
#define SOMETHING_AABB_BODY_HPP_

enum class Direction {
    Right = 0,
    Left,
};

constexpr V2<float> direction_to_v2(Direction direction)
{
    switch (direction) {
    case Direction::Right:
        return V2(1.0f, 0.0f);
    case Direction::Left:
        return V2(-1.0f, 0.0f);
    default:
        return V2(0.0f);
    }
}

struct AABB_Body {
    AABB<float> hitbox;
    V2<float> vel;

    inline V2<float> center() const
    {
        return hitbox.pos + hitbox.size * 0.5f;
    }

    void update(const Game *game, Seconds dt);
    void move(Direction direction, float speed);
    void stop();

    static AABB_Body from_hitbox(AABB<float> hitbox);
};

#endif // SOMETHING_AABB_BODY_HPP_
