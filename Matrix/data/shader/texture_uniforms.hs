uniform int use_texture;
uniform sampler2D diffuse_texture;
uniform samplerCube texture_cube;

in vec2 fragTexCoord;

vec4 getDiffuseTexel() {
  if(use_texture == 1)
    return texture(diffuse_texture, fragTexCoord);
  else
    return vec4(1.0);
}
