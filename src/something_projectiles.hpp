#ifndef SOMETHING_PROJECTILES_HPP_
#define SOMETHING_PROJECTILES_HPP_

#include "something_rgba.hpp"

struct Projectiles
{
    constexpr static size_t CAPACITY = 1024;
    constexpr static float RADIUS = 20.0f;
    constexpr static RGBA COLOR = RGBA::RED();
    constexpr static Seconds LIFETIME = 5.0f;

    enum class State {
        Ded = 0,
        Aliv
    };

    V2<float> positions[CAPACITY];
    V2<float> velocities[CAPACITY];
    State states[CAPACITY];
    Seconds lifetimes[CAPACITY];

    void update(Game *game, Seconds dt);
    void render(Renderer *renderer) const;

    void push(V2<float> pos, V2<float> vel);
};

#endif // SOMETHING_PROJECTILES_HPP_
