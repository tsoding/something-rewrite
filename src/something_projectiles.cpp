#include "./something_projectiles.hpp"

void Projectiles::update(Game *, Seconds dt)
{
    for (size_t i = 0; i < CAPACITY; ++i) {
        if (states[i] != State::Ded) {
            positions[i] += velocities[i] * dt;
            lifetimes[i] -= dt;
            if (lifetimes[i] <= 0) {
                states[i] = State::Ded;
            }
        }
    }
}

void Projectiles::render(Renderer *renderer) const
{
    for (size_t i = 0; i < CAPACITY; ++i) {
        if (states[i] == State::Aliv) {
            renderer->fill_triangle(
                equilateral_triangle(
                    positions[i],
                    RADIUS,
                    angle_v2(velocities[i])), COLOR, {});
        }
    }
}

void Projectiles::push(V2<float> pos, V2<float> vel)
{
    for (size_t i = 0; i < CAPACITY; ++i) {
        if (states[i] == State::Ded) {
            positions[i] = pos;
            velocities[i] = vel;
            states[i] = State::Aliv;
            lifetimes[i] = LIFETIME;
            return;
        }
    }
}
