#version 300 es

precision mediump float;

uniform sampler2D atlas;

in vec4 color;
in vec2 uv;
out vec4 output_color;

void main() {
    vec4 pixel = texture(atlas, uv);
    output_color = vec4(mix(pixel.xyz, color.xyz, color.w), pixel.w);
}
