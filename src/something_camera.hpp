#ifndef SOMETHING_CAMERA_HPP_
#define SOMETHING_CAMERA_HPP_

struct Camera
{
    V2<float> resolution;
    V2<float> position;
    float z;

    V2<float> to_screen(V2<float> world) const;
    AABB<float> to_screen(AABB<float> world) const;
};

#endif // SOMETHING_CAMERA_HPP_
