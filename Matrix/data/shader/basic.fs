#version 330
#extension GL_ARB_separate_shader_objects: enable

precision highp float;

in vec3 normal;
in vec2 tex_coord;
out vec4 color;

#include <data/shader/matrix_core.hs>
#include <data/shader/material_uniforms.hs>
#include <data/shader/texture_uniforms.hs>

void main() {
  vec3 light_dir = vec3(0.0, 0.5, sin(time * 0.01) * 5);

  vec3 n = normalize(normal);
  vec3 l = normalize(light_dir);
  float cosTheta = clamp( dot(n, l), 0, 1);

  vec3 incident = normalize(vec3(0.0, 1.0, 0.5));
  vec3 reflected = reflect(incident, n);

  vec4 texel = getDiffuseTexel();

  if(emissive == 1)
    cosTheta = 1.0;

  if(use_texture == 0) {
    color = mat_ambient + mat_diffuse*cosTheta;
    color.w = mat_ambient.w;
  } else {
    color = mat_ambient*texel + mat_diffuse*cosTheta*texel;
    color.w = texel.w;
  }

  if(use_reflection == 1)
    color *= texture(texture_cube, reflected);
}
