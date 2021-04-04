#include "./something_renderer.hpp"

void Renderer::fill_rect(AABB<float> aabb, RGBA shade, AABB<float> uv_aabb)
{
    Triangle<GLfloat> lower, upper;
    aabb.split_into_triangles(&lower, &upper);

    Triangle<GLfloat> lower_uv, upper_uv;
    uv_aabb.split_into_triangles(&lower_uv, &upper_uv);

    fill_triangle(lower, shade, lower_uv);
    fill_triangle(upper, shade, upper_uv);
}

void Renderer::fill_triangle(Triangle<GLfloat> triangle, RGBA rgba, Triangle<GLfloat> uv)
{
    // NOTE: I'm not sure if we should ignore the call if the buffer is full or crash.
    // Crash can help to troubleshoot disappearing triangles problem in the future.
    assert(batch_buffer_size < BATCH_BUFFER_CAPACITY);
    triangles_buffer[batch_buffer_size] = triangle;
    colors_buffer[batch_buffer_size][0] = rgba;
    colors_buffer[batch_buffer_size][1] = rgba;
    colors_buffer[batch_buffer_size][2] = rgba;
    uv_buffer[batch_buffer_size] = uv;
    batch_buffer_size += 1;
}

void Renderer::init()
{
    println(stderr, "LOG: initializing vertex position attribute");
    // Initializing Vertex Position Attribute
    {
        const size_t V2_COMPONENTS = 2;
        glGenBuffers(1, &triangles_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, triangles_buffer_id);
        {
            const size_t TRIANGLE_VERTICES = 3;
            static_assert(
                sizeof(triangles_buffer) == sizeof(GLfloat) * V2_COMPONENTS * TRIANGLE_VERTICES * BATCH_BUFFER_CAPACITY,
                "Looks like compiler did an oopsie-doopsie and padded something incorrectly in the Triangle or V2 structures");
        }
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(triangles_buffer),
                     triangles_buffer,
                     GL_DYNAMIC_DRAW);
        const GLint vertex_position = 1;
        println(stderr, "vertex_position: ", vertex_position);
        glEnableVertexAttribArray(vertex_position);

        glVertexAttribPointer(
            vertex_position,    // index
            V2_COMPONENTS,      // numComponents
            GL_FLOAT,           // type
            0,                  // normalized
            0,                  // stride
            0                   // offset
        );
    }

    println(stderr, "LOG: initializing vertex color attribute");
    // Initializing Vertex Color Attribute
    {
        const size_t RGBA_COMPONENTS = 4;
        glGenBuffers(1, &colors_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, colors_buffer_id);
        {
            const size_t TRIANGLE_VERTICES = 3;
            static_assert(
                sizeof(colors_buffer) == sizeof(GLfloat) * RGBA_COMPONENTS * TRIANGLE_VERTICES * BATCH_BUFFER_CAPACITY,
                "Looks like compiler did an oopsie-doopsie and padded something incorrectly in the RGBA structure");
        }
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(colors_buffer),
                     colors_buffer,
                     GL_DYNAMIC_DRAW);
        const GLint vertex_color = 2;
        println(stderr, "vertex_color: ", vertex_color);

        glEnableVertexAttribArray(vertex_color);
        glVertexAttribPointer(
            vertex_color,       // index
            RGBA_COMPONENTS,    // numComponents
            GL_FLOAT,           // type
            0,                  // normalized
            0,                  // stride
            0                   // offset
        );
    }

    println(stderr, "LOG: initializing UV position attribute");
    // Initializing Vertex UV Attribute
    {
        const size_t V2_COMPONENTS = 2;
        glGenBuffers(1, &uv_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id);
        {
            const size_t TRIANGLE_VERTICES = 3;
            static_assert(
                sizeof(uv_buffer) == sizeof(GLfloat) * V2_COMPONENTS * TRIANGLE_VERTICES * BATCH_BUFFER_CAPACITY,
                "Looks like compiler did an oopsie-doopsie and padded something incorrectly in the Triangle or V2 structures");
        }
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(uv_buffer),
                     uv_buffer,
                     GL_DYNAMIC_DRAW);
        const GLint vertex_uv = 3;
        println(stderr, "vertex_uv: ", vertex_uv);
        glEnableVertexAttribArray(vertex_uv);

        glVertexAttribPointer(
            vertex_uv,    // index
            V2_COMPONENTS,      // numComponents
            GL_FLOAT,           // type
            0,                  // normalized
            0,                  // stride
            0                   // offset
        );
    }
}

void Renderer::present()
{
    glBindBuffer(GL_ARRAY_BUFFER, triangles_buffer_id);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(triangles_buffer[0]) * batch_buffer_size,
                    triangles_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, colors_buffer_id);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(colors_buffer[0]) * batch_buffer_size,
                    colors_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(uv_buffer[0]) * batch_buffer_size,
                    uv_buffer);

    glDrawArrays(GL_TRIANGLES,
                 0,
                 static_cast<GLsizei>(batch_buffer_size) * TRIANGLE_VERT_COUNT);

    batch_buffer_size = 0;
}

