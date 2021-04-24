#include "./something_triangle_vao.hpp"

void Triangle_VAO::fill_aabb(AABB<float> aabb, RGBA shade, AABB<float> uv_aabb)
{
    Triangle<GLfloat> lower, upper;
    aabb.split_into_triangles(&lower, &upper);

    Triangle<GLfloat> lower_uv, upper_uv;
    uv_aabb.split_into_triangles(&lower_uv, &upper_uv);

    fill_triangle(lower, shade, lower_uv);
    fill_triangle(upper, shade, upper_uv);
}

void Triangle_VAO::fill_triangle(Triangle<GLfloat> triangle, RGBA rgba, Triangle<GLfloat> uv)
{
    // NOTE: I'm not sure if we should ignore the call if the buffer is full or crash.
    // Crash can help to troubleshoot disappearing triangles problem in the future.
    assert(count < CAPACITY);
    triangles[count] = triangle;
    colors[count][0] = rgba;
    colors[count][1] = rgba;
    colors[count][2] = rgba;
    uvs[count]       = uv;
    count += 1;
}

void Triangle_VAO::init()
{
    vbo_element_size[TRIANGLE_ATTRIB] = sizeof(triangles[0]);
    vbo_element_size[COLORS_ATTRIB] = sizeof(colors[0]);
    vbo_element_size[UV_ATTRIB] = sizeof(uvs[0]);
    static_assert(COUNT_ATTRIBS == 3);

    vbo_datas[TRIANGLE_ATTRIB] = triangles;
    vbo_datas[COLORS_ATTRIB] = colors;
    vbo_datas[UV_ATTRIB] = uvs;
    static_assert(COUNT_ATTRIBS == 3);

    attrib_size[TRIANGLE_ATTRIB] = V2_COMPONENTS;
    attrib_size[COLORS_ATTRIB] = RGBA_COMPONENTS;
    attrib_size[UV_ATTRIB] = V2_COMPONENTS;
    static_assert(COUNT_ATTRIBS == 3);

    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
    glGenBuffers(COUNT_ATTRIBS, vbo_ids);

    for (GLuint attrib = 0; attrib < COUNT_ATTRIBS; ++attrib) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[attrib]);
        glBufferData(GL_ARRAY_BUFFER,
                     vbo_element_size[attrib] * CAPACITY,
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

void Triangle_VAO::draw()
{
    glBindVertexArray(id);

    for (GLuint attrib = 0; attrib < COUNT_ATTRIBS; ++attrib) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[attrib]);
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        vbo_element_size[attrib] * count,
                        vbo_datas[attrib]);
    }

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(count) * TRIANGLE_VERT_COUNT);
}

void Triangle_VAO::clear()
{
    count = 0;
}
