#version 330 core

precision mediump float;

uniform sampler2D image;
uniform float time;

in vec4 color;
in vec2 uv;
out vec4 output_color;

void main() {
    output_color = texture(image, uv) * color;
}
