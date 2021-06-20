#ifndef SOMETHING_TRIANGLE_VAO_HPP_
#define SOMETHING_TRIANGLE_VAO_HPP_

#include "./something_math.hpp"
#include "./something_color.hpp"
#include "./something_camera.hpp"

struct Triangle_VAO {
    static const size_t CAPACITY = 1024;

    GLuint id;

    enum Attrib: GLuint {
        TRIANGLE_ATTRIB = 0,
        COLORS_ATTRIB,
        UV_ATTRIB,
        COUNT_ATTRIBS,
    };

    GLuint vbo_ids[COUNT_ATTRIBS];
    GLuint vbo_element_size[COUNT_ATTRIBS];
    void *vbo_datas[COUNT_ATTRIBS];
    GLint attrib_size[COUNT_ATTRIBS];
 
    Triangle<GLfloat> triangles[CAPACITY];
    RGBA colors[CAPACITY][3];
    Triangle<GLfloat> uvs[CAPACITY];
    size_t count;

    void init();
    void use();
    void sync_buffers();
    void fill_triangle(Triangle<GLfloat> triangle, RGBA rgba, Triangle<GLfloat> uv);
    void fill_aabb(AABB<float> aabb, RGBA shade, AABB<float> uv_aabb);
    void draw();
    void clear();
};

#endif  // SOMETHING_TRIANGLE_VAO_HPP_
