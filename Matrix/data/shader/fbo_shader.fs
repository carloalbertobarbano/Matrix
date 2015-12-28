#version 330

uniform sampler2D fbo_texture;
uniform float time;

in vec2 glTexCoord;
out vec4 color;

void main() {
  color.xyz = texture2D(fbo_texture, glTexCoord).xyz; //texture2D( fbo_texture, glTexCoord + 0.005*vec2( sin(time+1024.0*glTexCoord.x),cos(time+768.0*glTexCoord.y)) ).xyz;
  color.w = 1.0;
}
