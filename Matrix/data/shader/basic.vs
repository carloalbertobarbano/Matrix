#version 330
#extension GL_ARB_separate_shader_objects: enable

#include <data/shader/layout_declare>

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
