#version 330 core

precision mediump float;

uniform sampler2D image;
uniform float time;

in vec4 color;
in vec2 uv;
out vec4 output_color;

vec3 hsl2rgb(vec3 c) {
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0);
    return c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0));
}

void main() {
    output_color = texture(image, uv) * vec4(hsl2rgb(vec3((time - uv.x - uv.y) * 0.5, 1.0, 0.80)), color.w);
}
