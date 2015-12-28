#version 330

layout(location = 0)in vec3 vert;
layout(location = 1)in vec2 tex_coord;

out vec2 glTexCoord;

void main() {
  glTexCoord = tex_coord;

  gl_Position.xyz = vert;
  gl_Position.w = 1.0;
}
