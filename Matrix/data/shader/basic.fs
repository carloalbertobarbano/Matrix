#version 330
#extension GL_ARB_separate_shader_objects: enable

precision highp float;

uniform mat4 modMat;
uniform vec3 cameraPosition;

out vec4 fragColor;

#include <data/shader/matrix_core.hs>
#include <data/shader/material_uniforms.hs>
#include <data/shader/texture_uniforms.hs>
#include <data/shader/lighting.fs>

void main() {
  computeLighting();

  vec4 texel = getDiffuseTexel();
  if(use_texture == 0)
    texel = vec4(1.0);

  fragColor =  mat_ambient*texel + vec4(diffuseComponent * texel.rgb, 1.0) + vec4(specularComponent * texel.rgb, 1.0);
  fragColor.a = texel.a;
}
