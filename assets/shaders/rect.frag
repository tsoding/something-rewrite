#version 300 es

precision mediump float;

uniform sampler2D atlas;
uniform float time;

in vec4 color;
in vec2 uv;
out vec4 output_color;

void main() {
    output_color = texture(atlas, uv) * color;
}
