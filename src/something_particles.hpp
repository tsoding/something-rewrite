#ifndef SOMETHING_PARTICLES_HPP_
#define SOMETHING_PARTICLES_HPP_

struct Particles
{
    static constexpr size_t CAPACITY = 10;
    static constexpr float MIN_MAG = 1.0f;
    static constexpr float MAX_MAG = 10.0f;
    static constexpr float SIZE = 100.0f;
    static constexpr RGBA COLOR = RGBA::RED();
    static_assert(MIN_MAG <= MAX_MAG);

    V2<float> positions[CAPACITY];
    V2<float> velocities[CAPACITY];
    size_t count;

    void push(V2<float> position);
    void update(Seconds delta_time);
    void render(Triangle_Renderer *renderer) const;
};

#endif // SOMETHING_PARTICLES_HPP_
