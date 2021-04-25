#include "./something_anim.hpp"

namespace anim
{

float Player::update(float dt)
{
    if (segment_current < segments_count) {
        segment_time += dt;
        while (segment_current < segments_count && segment_time >= segments[segment_current].duration) {
            segment_time -= segments[segment_current].duration;
            segment_current += 1;
        }

        if (segment_current < segments_count) {
            const Segment &segment = segments[segment_current];
            const float t = segment_time / segment.duration;
            return lerp(segment.begin,
                        segment.end,
                        segment.f ? segment.f(t) : t);
        }
    }

    assert(segments_count > 0);
    return segments[segments_count - 1].end;
}

bool Player::is_finished() const
{
    return segment_current >= segments_count;
}

void Player::reset()
{
    segment_current = 0;
    segment_time = 0.0f;
}

}  // namespace anim
