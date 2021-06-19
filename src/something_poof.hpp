#ifndef SOMETHING_POOF_HPP_
#define SOMETHING_POOF_HPP_

struct Poof {
    static constexpr Seconds LIFETIME = 3.0f;
    static constexpr size_t CAPACITY = 1024;
    static constexpr float MAX_VELOCITY = 200.0f;
    static constexpr float MAX_ANGLE_VELOCITY = static_cast<float>(M_PI);

    Triangle<float>  triangles[CAPACITY];
    RGBA             shades[CAPACITY];
    Triangle<float>  uvs[CAPACITY];

    V2<float>        positions[CAPACITY];
    V2<float>        velocities[CAPACITY];

    float            angles[CAPACITY];
    float            angle_velocities[CAPACITY];
    V2<float>        pivots[CAPACITY];

    Seconds          lifetimes[CAPACITY];

    size_t           last;

    void push(Triangle<float> triangle,
              RGBA shade,
              Triangle<float> uv);
    void update(Game *game, Seconds dt);
    void explode_triangle(Triangle<float> vert, RGBA shade, Triangle<float> uv, int level);
};

#endif // SOMETHING_POOF_HPP_
