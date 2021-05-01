#version 330 core

precision mediump float;

uniform vec2 resolution;
uniform float time;
uniform vec2 camera_position;
uniform float camera_zoom;

layout(location = 0) in vec2 circle_center;
layout(location = 1) in float circle_radius;
layout(location = 2) in vec4 circle_color;

out vec4 color;
out vec2 uv;

vec2 camera_projection(vec2 position)
{
    return 2.0 * (position - camera_position) / (resolution * camera_zoom);
}

void main() {
    uv = vec2(float(gl_VertexID & 1),
              float((gl_VertexID >> 1) & 1));

    gl_Position =
        vec4(
            camera_projection(circle_center + (2.0 * uv - 1.0) * circle_radius),
            0.0,
            1.0);
    color = circle_color;
}
