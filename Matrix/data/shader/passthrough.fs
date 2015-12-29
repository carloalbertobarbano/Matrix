#version 150
#extension GL_ARB_separate_shader_objects: enable

precision highp float;

in vec3 color;

out vec4 fragColor;


void main() {
   fragColor = vec4(color, 1.0);
}
