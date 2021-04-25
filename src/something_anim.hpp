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

    float update(float dt);
    bool is_finished() const;
    void reset();
};

}  // namespace anim

#endif // SOMETHING_ANIM_HPP_
