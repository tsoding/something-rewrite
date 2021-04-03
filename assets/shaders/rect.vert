#version 300 es

precision mediump float;

uniform vec2 resolution;
uniform float time;
uniform vec2 camera_position;
uniform float camera_scale;

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

vec2 camera_projection(vec2 pos)
{
    float x = lerp(
        -1.0, 1.0,
        inv_lerp(
            -(resolution.x * camera_scale) * 0.5,
            (resolution.x * camera_scale) * 0.5, 
            pos.x));
    float y = lerp(
        -1.0, 1.0, 
        inv_lerp(
            -(resolution.y * camera_scale) * 0.5,
            (resolution.y * camera_scale) * 0.5,
            pos.y));
    return vec2(x, y);
}

void main() {
    gl_Position = vec4(
        camera_projection(vertex_position - camera_position), 
        0.0, 1.0);
    color = vertex_color;
    uv = vertex_uv;
}
