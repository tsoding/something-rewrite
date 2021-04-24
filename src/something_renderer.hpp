#ifndef SOMETHING_RENDERER_HPP_
#define SOMETHING_RENDERER_HPP_

#include "./something_geo.hpp"
#include "./something_rgba.hpp"
#include "./something_camera.hpp"

struct Renderer {
    static const size_t BATCH_BUFFER_CAPACITY = 1024;

    // Buffers
    GLuint triangles_buffer_id;
    GLuint colors_buffer_id;
    GLuint uv_buffer_id;

    Triangle<GLfloat> triangles_buffer[BATCH_BUFFER_CAPACITY];
    RGBA colors_buffer[BATCH_BUFFER_CAPACITY][3];
    Triangle<GLfloat> uv_buffer[BATCH_BUFFER_CAPACITY];
    size_t batch_buffer_size;

    void init();
    void fill_triangle(Triangle<GLfloat> triangle, RGBA rgba, Triangle<GLfloat> uv);
    void fill_rect(AABB<float> aabb, RGBA shade, AABB<float> uv_aabb);
    void draw();
    void clear();
};

#endif  // SOMETHING_RENDERER_HPP_
