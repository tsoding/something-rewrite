#version 330 core

precision mediump float;

uniform float time;

in vec4 color;
in vec2 uv;
out vec4 output_color;

void main() {
    float t = clamp(distance(uv, vec2(0.5, 0.5)) * 2.0, 0.0, 1.0);
    output_color = vec4(color.xyz, 1.0 - smoothstep(0.0, 1.0, t));
}
