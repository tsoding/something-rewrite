#ifndef SOMETHING_CAMERA_HPP_
#define SOMETHING_CAMERA_HPP_

struct Camera
{
    V2<float> pos;
    V2<float> vel;
    float z;

    void update(Seconds dt);
};

#endif // SOMETHING_CAMERA_HPP_
