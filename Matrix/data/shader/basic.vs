#version 330
#extension GL_ARB_separate_shader_objects: enable

#include <data/shader/layout_declare>

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modMat;

out vec3 fragVert;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main() {
  fragTexCoord = tex;
  fragNormal = norm;
  fragVert = vert;

  gl_Position =  projMat * viewMat * modMat * vec4(vert, 1.0);
}
