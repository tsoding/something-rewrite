#ifndef SOMETHING_CAMERA_HPP_
#define SOMETHING_CAMERA_HPP_

struct Camera
{
    constexpr static float DISTANCE = 100.0f;

    V2<float> pos;
    V2<float> vel;
    float zoom;

    void update(Seconds dt);
};

#endif // SOMETHING_CAMERA_HPP_
