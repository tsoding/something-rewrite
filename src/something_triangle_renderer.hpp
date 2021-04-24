#ifndef SOMETHING_TRIANGLE_RENDERER_HPP_
#define SOMETHING_TRIANGLE_RENDERER_HPP_

#include "./something_geo.hpp"
#include "./something_rgba.hpp"
#include "./something_camera.hpp"

struct Triangle_Renderer {
    static const size_t BATCH_BUFFER_CAPACITY = 1024;

    GLuint vao_id;

    enum Attrib: int {
        TRIANGLE_ATTRIB = 0,
        COLORS_ATTRIB,
        UV_ATTRIB,
        COUNT_ATTRIBS,
    };

    GLuint vbo_ids[COUNT_ATTRIBS];
    GLuint vbo_element_size[COUNT_ATTRIBS];
    void *vbo_datas[COUNT_ATTRIBS];
    GLint attrib_size[COUNT_ATTRIBS];
 
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

#endif  // SOMETHING_TRIANGLE_RENDERER_HPP_
