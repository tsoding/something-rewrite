#include "./something_anim.hpp"

namespace anim
{

float Player::update(float dt)
{
    const Segment *segment = &segments[segment_current];

    segment_time += dt;
    while (segment_time >= segment->duration) {
        segment_time -= segment->duration;

        if (segment_current + 1 >= segments_count) {
            looped_count += 1;
        }

        segment_current = (segment_current + 1) % segments_count;

        segment = &segments[segment_current];
    }

    const float t = segment_time / segment->duration;
    return lerp(segment->begin,
                segment->end,
                segment->f ? segment->f(t) : t);
}

}  // namespace anim
