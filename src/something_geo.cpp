#include "./something_geo.hpp"

V2<float> rotate_v2(V2<float> v, float angle)
{
    return V2(cos(angle) * v.x - sin(angle) * v.y,
              sin(angle) * v.x + cos(angle) * v.y);
}

float length(V2<float> v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

V2<float> lerp(V2<float> a, V2<float> b, float f)
{
    return a + (b - a) * f;
}

void split_triangle(const Triangle<float> &tri,
                    const float fs[TRIANGLE_VERT_COUNT],
                    Triangle<float> output[TRIANGLE_VERT_COUNT + 1])
{
    V2<float> ps[TRIANGLE_VERT_COUNT] = {0};
    for (size_t i = 0; i < TRIANGLE_VERT_COUNT; ++i) {
        ps[i] = lerp(tri.vs[i], tri.vs[(i + 1) % TRIANGLE_VERT_COUNT], fs[i]);
    }

    for (size_t i = 0; i < TRIANGLE_VERT_COUNT; ++i) {
        output[i] = Triangle(tri.vs[i], ps[i], ps[(i + 2) % TRIANGLE_VERT_COUNT]);
    }
    output[TRIANGLE_VERT_COUNT] = Triangle(ps[0], ps[1], ps[2]);
}

void split_triangle(const Triangle<float> &tri,
                    size_t side, float f,
                    Triangle<float> output[2])
{
    assert(side < TRIANGLE_VERT_COUNT);
    assert(0.0f <= f && f <= 1.0f);

    const auto a = tri.vs[side];
    const auto b = tri.vs[(side + 1) % TRIANGLE_VERT_COUNT];
    const auto c = tri.vs[(side + 2) % TRIANGLE_VERT_COUNT];
    const auto q = lerp(a, b, f);

    output[0] = Triangle(c, q, a);
    output[1] = Triangle(c, q, b);
}

Triangle<float> rotate_triangle(Triangle<float> tri, float angle, V2<float> pivot)
{
    auto result = tri - pivot;
    for (auto &v : result.vs) {
        v = rotate_v2(v, angle);
    }
    return result + pivot;
}
