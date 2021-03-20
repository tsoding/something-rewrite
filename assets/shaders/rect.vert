#version 300 es

precision mediump float;

uniform vec2 resolution;
uniform float time;
uniform vec2 camera_position;
uniform float camera_z;

layout(location = 1) in vec2 vertex_position;
layout(location = 2) in vec4 vertex_color;
layout(location = 3) in vec2 vertex_uv;

out vec4 color;
out vec2 uv;

float inv_lerp(float a, float b, float q)
{
    return (q - a) / (b - a);
}

float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

void main() {
    float aspect = resolution.x / resolution.y;
    vec2 pos = vertex_position - camera_position;
    // TODO: camera_z is not used for anything
    float x = lerp(-1.0, 1.0, inv_lerp(-resolution.x * 0.5, resolution.x * 0.5, pos.x)) * aspect;
    float y = lerp(-1.0, 1.0, inv_lerp(-resolution.y * 0.5, resolution.y * 0.5, pos.y)) * aspect;

    gl_Position = vec4(x, y, 0.0, 1.0);
    color = vertex_color;
    uv = vertex_uv;
}
