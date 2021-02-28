#ifndef SOMETHING_RENDERER_HPP_
#define SOMETHING_RENDERER_HPP_

#include "./something_geo.hpp"
#include "./something_rgba.hpp"

struct Renderer {
    static const size_t BATCH_BUFFER_CAPACITY = 1024;

    // The GLSL program that can render a rectangle
    GLuint rect_program;

    // Atlas
    GLuint u_atlas;
    Atlas atlas;

    // Buffers
    GLuint triangles_buffer_id;
    GLuint colors_buffer_id;
    GLuint uv_buffer_id;

    Triangle<GLfloat> triangles_buffer[BATCH_BUFFER_CAPACITY];
    RGBA colors_buffer[BATCH_BUFFER_CAPACITY][3];
    Triangle<GLfloat> uv_buffer[BATCH_BUFFER_CAPACITY];
    size_t batch_buffer_size;

    Fixed_Region<1000 * 1000> shader_buffer;

    void init(const char *atlas_conf_path);
    void fill_triangle(Triangle<GLfloat> triangle, RGBA rgba, Triangle<GLfloat> uv);
    void fill_rect(AABB<float> aabb, RGBA shade, int atlas_index);
    void present();

    GLuint gl_compile_shader_file(const char *file_path, GLenum shader_type);
    GLuint gl_link_program(GLuint *shader, size_t shader_size);
};

#endif  // SOMETHING_RENDERER_HPP_
