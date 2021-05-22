// A fragment shader that textures the fragment according to the
// provided `image` and `uv` coordinates and multiplies it by the
// `color`
#version 330 core

precision mediump float;

uniform sampler2D image;

in vec4 color;
in vec2 uv;
out vec4 output_color;

void main() {
    output_color = texture(image, uv) * color;
}
