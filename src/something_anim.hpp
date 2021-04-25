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
    size_t segment_index;
    Seconds segment_time;
    size_t looped_count;

    float update(float dt, const Segment *animation, size_t animation_size);
};

}  // namespace anim

#endif // SOMETHING_ANIM_HPP_
