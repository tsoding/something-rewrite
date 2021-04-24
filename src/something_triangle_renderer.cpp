#include "./something_triangle_renderer.hpp"

void Triangle_Renderer::fill_rect(AABB<float> aabb, RGBA shade, AABB<float> uv_aabb)
{
    Triangle<GLfloat> lower, upper;
    aabb.split_into_triangles(&lower, &upper);

    Triangle<GLfloat> lower_uv, upper_uv;
    uv_aabb.split_into_triangles(&lower_uv, &upper_uv);

    fill_triangle(lower, shade, lower_uv);
    fill_triangle(upper, shade, upper_uv);
}

void Triangle_Renderer::fill_triangle(Triangle<GLfloat> triangle, RGBA rgba, Triangle<GLfloat> uv)
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

void Triangle_Renderer::init()
{
    vbo_element_size[TRIANGLE_ATTRIB] = sizeof(triangles_buffer[0]);
    vbo_element_size[COLORS_ATTRIB] = sizeof(colors_buffer[0]);
    vbo_element_size[UV_ATTRIB] = sizeof(uv_buffer[0]);
    static_assert(COUNT_ATTRIBS == 3);

    vbo_datas[TRIANGLE_ATTRIB] = triangles_buffer;
    vbo_datas[COLORS_ATTRIB] = colors_buffer;
    vbo_datas[UV_ATTRIB] = uv_buffer;
    static_assert(COUNT_ATTRIBS == 3);

    attrib_size[TRIANGLE_ATTRIB] = V2_COMPONENTS;
    attrib_size[COLORS_ATTRIB] = RGBA_COMPONENTS;
    attrib_size[UV_ATTRIB] = V2_COMPONENTS;
    static_assert(COUNT_ATTRIBS == 3);

    glGenVertexArrays(1, &vao_id);
    glGenBuffers(COUNT_ATTRIBS, vbo_ids);

    for (int attrib = 0; attrib < COUNT_ATTRIBS; ++attrib) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[attrib]);
        glBufferData(GL_ARRAY_BUFFER,
                     vbo_element_size[attrib] * BATCH_BUFFER_CAPACITY,
                     vbo_datas[attrib],
                     GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(attrib);

        glVertexAttribPointer(
            attrib,    // index
            attrib_size[attrib],      // numComponents
            GL_FLOAT,           // type
            0,                  // normalized
            0,                  // stride
            0                   // offset
        );
    }
}

void Triangle_Renderer::draw()
{
    for (int attrib = 0; attrib < COUNT_ATTRIBS; ++attrib) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[attrib]);
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        vbo_element_size[attrib] * batch_buffer_size,
                        vbo_datas[attrib]);
    }

    glDrawArrays(GL_TRIANGLES,
                 0,
                 static_cast<GLsizei>(batch_buffer_size) * TRIANGLE_VERT_COUNT);
}

void Triangle_Renderer::clear()
{
    batch_buffer_size = 0;
}
