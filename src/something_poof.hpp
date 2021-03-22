#ifndef SOMETHING_POOF_HPP_
#define SOMETHING_POOF_HPP_

struct Poof {
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

    size_t poof_size;

    void push(Triangle<float> triangle,
              RGBA shade,
              Triangle<float> uv);
    void clear();
    void update(Seconds dt);
    void render(Renderer *renderer) const;
};

#endif // SOMETHING_POOF_HPP_
