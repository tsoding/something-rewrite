#ifndef SOMETHING_AABB_BODY_HPP_
#define SOMETHING_AABB_BODY_HPP_

struct AABB_Body {
    AABB<float> hitbox;
    V2<float> vel;

    inline V2<float> center() const
    {
        return hitbox.pos + hitbox.size * 0.5f;
    }

    void update(const Game *game, Seconds dt);
};

#endif // SOMETHING_AABB_BODY_HPP_
