#include "./something_particles.hpp"

void Particles::push(V2<float> position)
{
    if (count < CAPACITY) {
        positions[count] = position;
        velocities[count] =
            polar_v2(lerp(0.0f, 2.0f * static_cast<float>(M_PI), random01()),
                     lerp(MIN_MAG, MAX_MAG, random01()));
        count += 1;
    }
}

void Particles::update(Seconds delta_time)
{
    for (size_t i = 0; i < count; ++i) {
        positions[i] += velocities[i] * delta_time;
    }
}

void Particles::render(Circle_Renderer *circle_renderer) const
{
    for (size_t i = 0; i < count; ++i) {
        circle_renderer->fill_circle(positions[i], SIZE, COLOR);
    }
}
