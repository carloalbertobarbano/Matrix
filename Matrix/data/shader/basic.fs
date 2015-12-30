#version 330
#extension GL_ARB_separate_shader_objects: enable

precision highp float;

uniform sampler2D diffuse_texture;
uniform float time;

in vec3 normal;
in vec2 tex_coord;

out vec4 color;

vec4 getTexel() {
   return texture(diffuse_texture, tex_coord);
}

void main() {
  vec3 light_dir = vec3(0.0, 0.5, 5);

  vec3 n = normalize(normal);
  vec3 l = normalize(light_dir);
  float cosTheta = clamp( dot(n, l), 0, 1);
  vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

  color = ambient*getTexel() + cosTheta*getTexel();
  color.w = 1.0;
}
