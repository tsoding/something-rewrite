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
                    size_t side, float f,
                    Triangle<float> *left,
                    Triangle<float> *right)
{
    assert(side < TRIANGLE_VERT_COUNT);
    assert(0.0f <= f && f <= 1.0f);

    const auto a = tri.vs[side];
    const auto b = tri.vs[(side + 1) % TRIANGLE_VERT_COUNT];
    const auto c = tri.vs[(side + 2) % TRIANGLE_VERT_COUNT];

    const auto q = lerp(a, b, f);

    if (left) {
        *left = Triangle(c, q, a);
    }

    if (right) {
        *right = Triangle(c, q, b);
    }
}

Triangle<float> rotate_triangle(Triangle<float> tri, float angle, V2<float> pivot)
{
    auto result = tri - pivot;
    for (auto &v : result.vs) {
        v = rotate_v2(v, angle);
    }
    return result + pivot;
}
