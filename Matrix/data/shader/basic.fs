#version 330
#extension GL_ARB_separate_shader_objects: enable

precision highp float;


uniform vec4 mat_ambient;
uniform vec4 mat_diffuse;
uniform vec4 mat_specular;
uniform int mat_shininess;

uniform int use_texture;
uniform sampler2D diffuse_texture;

uniform int emissive;

uniform float time;

in vec3 normal;
in vec2 tex_coord;

out vec4 color;

vec4 getTexel() {
   return texture(diffuse_texture, tex_coord);
}

void main() {
  vec3 light_dir = vec3(0.0, 0.5, sin(time * 0.01) * 5);

  vec3 n = normalize(normal);
  vec3 l = normalize(light_dir);
  float cosTheta = clamp( dot(n, l), 0, 1);

  vec4 texel = getTexel();

  if(use_texture == 0) {
    color = mat_diffuse*cosTheta;
    color.w = mat_ambient.w;
  } else {
    color = mat_ambient*texel + mat_diffuse*cosTheta*texel;
    color.w = texel.w;
  }

  if(emissive == 1)
    color = mat_ambient + mat_diffuse;

  //color.w = 1.0;
}
