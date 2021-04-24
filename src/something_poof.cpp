#include "./something_poof.hpp"

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
    for (size_t i = 0; i < CAPACITY; ++i) {
        if (lifetimes[i] <= 0.0f) {
            triangles[i] = triangle;
            shades[i]    = shade;
            uvs[i]       = uv;

            positions[i] = V2(0.0f);
            velocities[i] = polar_v2(random01() * 2.0f * static_cast<float>(M_PI), random01() * MAX_VELOCITY);

            angles[i] = 0.0f;
            angle_velocities[i] = random01() * MAX_ANGLE_VELOCITY;
            pivots[i] = point_on_triangle(triangle, random01(), random01());

            lifetimes[i] = LIFETIME;
            return;
        }
    }
}

void Poof::update(Seconds dt)
{
    for (size_t i = 0; i < CAPACITY; ++i) {
        if (lifetimes[i] > 0.0f) {
            positions[i] += velocities[i] * dt;
            angles[i] += angle_velocities[i] * dt;
            lifetimes[i] -= dt;
        }
    }
}

void Poof::render(Triangle_VAO *renderer) const
{
    for (size_t i = 0; i < CAPACITY; ++i) {
        if (lifetimes[i] > 0.0f) {
            renderer->fill_triangle(
                rotate_triangle(
                    triangles[i] + positions[i],
                    angles[i],
                    pivots[i] + positions[i]),
                shades[i].with_alpha(lifetimes[i] / LIFETIME),
                uvs[i]);
        }
    }
}
