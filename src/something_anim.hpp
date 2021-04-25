#ifndef SOMETHING_ANIM_HPP_
#define SOMETHING_ANIM_HPP_

namespace anim
{

struct Segment {
    float begin;
    float end;
    Seconds duration;
    float (*f)(float);
};

struct Player {
    const Segment *segments;
    size_t segments_count;

    size_t segment_current;
    Seconds segment_time;
    size_t looped_count;

    Player(const Segment *segments, size_t segments_count):
        segments(segments),
        segments_count(segments_count)
    {}

    float update(float dt);
};

}  // namespace anim

#endif // SOMETHING_ANIM_HPP_
