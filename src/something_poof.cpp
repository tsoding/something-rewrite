#include "./something_poof.hpp"

float random01()
{
    static_assert(RAND_MAX != 0);
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

V2<float> point_on_triangle(Triangle<float> triangle,
                            float f0, float f1)
{
    const auto a = triangle.vs[0];
    const auto b = triangle.vs[1];
    const auto c = triangle.vs[2];
    return lerp(a, lerp(b, c, f0), f1);
}

void Poof::push(Triangle<float> triangle,
                RGBA shade,
                Triangle<float> uv)
{
    assert(poof_size < CAPACITY);

    triangles[poof_size] = triangle;
    shades[poof_size]    = shade;
    uvs[poof_size]       = uv;

    positions[poof_size] = V2(0.0f);
    velocities[poof_size] = V2<float>::polar(random01() * 2.0f * M_PI, random01() * MAX_VELOCITY);

    angles[poof_size] = 0.0f;
    angle_velocities[poof_size] = random01() * MAX_ANGLE_VELOCITY;
    pivots[poof_size] = point_on_triangle(triangle, random01(), random01());

    poof_size += 1;
}

void Poof::clear()
{
    poof_size = 0;
}

void Poof::update(Seconds dt)
{
    for (size_t i = 0; i < poof_size; ++i) {
        positions[i] += velocities[i] * dt;
        angles[i] += angle_velocities[i] * dt;
    }
}

void Poof::render(Renderer *renderer) const
{
    for (size_t i = 0; i < poof_size; ++i) {
        renderer->fill_triangle(
            rotate_triangle(
                triangles[i] + positions[i],
                angles[i],
                pivots[i] + positions[i]),
            shades[i],
            uvs[i]);
    }
}
