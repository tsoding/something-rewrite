#version 300 es

precision mediump float;

uniform vec2 resolution;

layout(location = 1) in vec2 vertex_position;
layout(location = 2) in vec4 vertex_color;
layout(location = 3) in vec2 vertex_uv;

out vec4 color;
out vec2 uv;

void main() {
    float aspect = resolution.y / resolution.x;
    gl_Position = vec4(vertex_position * vec2(aspect, 1.0), 0.0, 1.0);
    color = vertex_color;
    uv = vertex_uv;
}
