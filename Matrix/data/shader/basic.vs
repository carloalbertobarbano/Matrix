#version 330
#extension GL_ARB_separate_shader_objects: enable

layout(location = 0)in vec3 vert;
layout(location = 1)in vec3 norm;
layout(location = 2)in vec2 tex;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modMat;

out vec3 normal;
out vec2 tex_coord;

void main() {
  normal = norm;
  tex_coord = tex;

  gl_Position =  projMat * viewMat * modMat * vec4(vert, 1.0);
}
