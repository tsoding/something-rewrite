#include "./something_poof.hpp"

V2<float> point_on_triangle(Triangle<float> triangle,
                            float f0, float f1)
{
    const auto a = triangle.vs[0];
    const auto b = triangle.vs[1];
    const auto c = triangle.vs[2];
    return lerp(a, lerp(b, c, V2(f0)), V2(f1));
}

void Poof::push(Triangle<float> triangle,
                RGBA shade,
                Triangle<float> uv)
{
    triangles[last] = triangle;
    shades[last]    = shade;
    uvs[last]       = uv;

    positions[last]  = V2(0.0f);
    velocities[last] = polar_v2(random01() * 2.0f * static_cast<float>(M_PI), random01() * MAX_VELOCITY);

    angles[last] = 0.0f;
    angle_velocities[last] = random01() * MAX_ANGLE_VELOCITY;
    pivots[last] = point_on_triangle(triangle, random01(), random01());

    lifetimes[last] = LIFETIME;

    last = (last + 1) % CAPACITY;
}

void Poof::update(Game *game, Seconds dt)
{
    for (size_t i = 0; i < CAPACITY; ++i) {
        if (lifetimes[i] > 0.0f) {
            positions[i] += velocities[i] * V2(dt);
            angles[i] += angle_velocities[i] * dt;
            lifetimes[i] -= dt;

            game->renderer.fill_triangle(
                rotate_triangle(
                    triangles[i] + positions[i],
                    angles[i],
                    pivots[i] + positions[i]),
                shades[i].with_alpha(lifetimes[i] / LIFETIME),
                uvs[i],
                PRIDE_PROGRAM_ASSET);
        }
    }
}

void Poof::explode_triangle(Triangle<float> vert, RGBA shade, Triangle<float> uv, int level)
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
            push(child_vert[i], shade, child_uv[i]);
        }
    } else {
        for (size_t i = 0; i < n; ++i) {
            explode_triangle(child_vert[i], shade, child_uv[i], level - 1);
        }
    }
}
