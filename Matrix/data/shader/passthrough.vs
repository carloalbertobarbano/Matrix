#version 150
#extension GL_ARB_separate_shader_objects: enable

in vec3 vert;
in vec3 vert_color;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modMat;

out vec3 color;

void main() {
  color = vert_color;

  gl_Position =  projMat * viewMat * modMat * vec4(vert, 1.0);
}
