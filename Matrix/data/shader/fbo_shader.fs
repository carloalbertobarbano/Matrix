#version 330

uniform sampler2D fbo_texture;
uniform float time;

in vec2 glTexCoord;
out vec4 fragColor;

void main() {
  fragColor.xyz = texture( fbo_texture, glTexCoord + 0.005*vec2( sin(time+1024.0*glTexCoord.x),cos(time+768.0*glTexCoord.y)) ).xyz;
  fragColor.w = 1.0;
}
