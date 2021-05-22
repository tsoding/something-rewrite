// Vertex shader that renders a triangle with a particular `uv`
// coordinates and `color` with applied camera_projection
#version 330 core

precision mediump float;

uniform vec2 resolution;
uniform float time;
uniform vec2 camera_position;
uniform float camera_zoom;

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec4 vertex_color;
layout(location = 2) in vec2 vertex_uv;

out vec4 color;
out vec2 uv;

vec2 camera_projection(vec2 position)
{
    return 2.0 * (position - camera_position) / (resolution * camera_zoom);
}

void main() {
    gl_Position = vec4(
        camera_projection(vertex_position), 
        0.0,
        1.0);
    color = vertex_color;
    uv = vertex_uv;
}
