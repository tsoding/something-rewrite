#include "./something_anim.hpp"

namespace anim
{

float Player::update(float dt, const Segment *animation, size_t animation_size)
{
    const Segment *segment = &animation[segment_index];

    segment_time += dt;
    while (segment_time >= segment->duration) {
        segment_time -= segment->duration;

        if (segment_index + 1 >= animation_size) {
            looped_count += 1;
        }

        segment_index = (segment_index + 1) % animation_size;

        segment = &animation[segment_index];
    }

    const float t = segment_time / segment->duration;
    return lerp(segment->begin,
                segment->end,
                segment->f ? segment->f(t) : t);
}

}  // namespace anim
