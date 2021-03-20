#include "./something_camera.hpp"

float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

float inv_lerp(float a, float b, float q)
{
    return (q - a) / (b - a);
}

V2<float> Camera::to_screen(V2<float> world) const
{
    // TODO: apply the camera transformations on the GPU (via shaders)
    const float aspect = resolution.x / resolution.y;
    world -= position;
    const float x = lerp(-1.0f, 1.0f, inv_lerp(-resolution.x * 0.5f, resolution.x * 0.5f, world.x)) * aspect;
    const float y = lerp(-1.0f, 1.0f, inv_lerp(-resolution.y * 0.5f, resolution.y * 0.5f, world.y));
    return V2(x, y);
}

AABB<float> Camera::to_screen(AABB<float> world) const
{
    const auto screen_pos1 = to_screen(world.pos);
    const auto screen_pos2 = to_screen(world.pos + world.size);
    return AABB(screen_pos1, screen_pos2 - screen_pos1);
}
