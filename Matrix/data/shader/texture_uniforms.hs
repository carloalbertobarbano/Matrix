uniform int use_texture;
uniform sampler2D diffuse_texture;
uniform samplerCube texture_cube;

vec4 getDiffuseTexel() {
   return texture(diffuse_texture, tex_coord);
}
