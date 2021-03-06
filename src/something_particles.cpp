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

void Particles::update(Game *game, Seconds delta_time)
{
    for (size_t i = 0; i < count; ++i) {
        positions[i] += velocities[i] * V2(delta_time);

        game->renderer.fill_aabb(
            AABB(positions[i] - V2(SIZE), V2(SIZE + SIZE)),
            COLOR,
            AABB(V2(0.0f), V2(1.0f)),
            PARTICLE_PROGRAM_ASSET);
    }
}
