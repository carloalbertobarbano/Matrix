#version 330
#extension GL_ARB_separate_shader_objects: enable

precision highp float;

in vec3 normal;
in vec2 tex_coord;
out vec4 color;

#include <data/shader/matrix_core.hs>
#include <data/shader/texture_uniforms.hs>


void main() {
  vec3 light_dir = vec3(0.0, 0.5, sin(time * 0.01) * 5);

  vec3 n = normalize(normal);
  vec3 l = normalize(light_dir);
  float cosTheta = clamp( dot(n, l), 0, 1);

  vec4 texel = getDiffuseTexel();
  vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

  color = ambient*texel + cosTheta*texel;
  color.w = 1.0;
}
