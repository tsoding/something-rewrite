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
    void update(Seconds dt);
    void render(Renderer *renderer) const;
};

void explode_triangle(Poof &poof, Triangle<float> vert, RGBA shade, Triangle<float> uv,
                      int level)
{
    Triangle<GLfloat> child_vert[4];
    Triangle<GLfloat> child_uv[4];
    size_t n = (rand() % 2 + 1) * 2;

    switch (n) {
    case 2: {
        size_t side = vert.longest_side();
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
            poof.push(child_vert[i], shade, child_uv[i]);
        }
    } else {
        for (size_t i = 0; i < n; ++i) {
            explode_triangle(poof, child_vert[i], shade, child_uv[i], level - 1);
        }
    }
}

#endif // SOMETHING_POOF_HPP_
