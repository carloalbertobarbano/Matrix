#include "Material.h"

Graphics::Components::Materials::Material::Material()
{
	ambient = vec4(0.0);
	ambient.w = 1.0;
	diffuse = vec4(0.0);
	diffuse.w = 1.0;
	specular = vec4(0.0);
	specular.w = 1.0;
	shininess = 50;

	reflection = false;
	refraction = false;
	emissive = false;

	name = "default_material";

	locations.constructed = false;
}

void Graphics::Components::Materials::Material::sendMaterialUniforms(ShaderProgram * shader)
{
	if (!locations.constructed) {
		locations.material_ambient = shader->getUniformLocation("mat_ambient");
		locations.material_diffuse = shader->getUniformLocation("mat_diffuse");
		locations.material_specular = shader->getUniformLocation("mat_specular");
		locations.material_shininess = shader->getUniformLocation("mat_shininess");
		locations.use_texture = shader->getUniformLocation("use_texture");
		locations.use_reflection = shader->getUniformLocation("use_reflection");
		locations.use_refraction = shader->getUniformLocation("use_refraction");
		locations.emissive = shader->getUniformLocation("emissive");
		locations.render_all_with_color = shader->getUniformLocation("render_all_with_color");
		locations.color = shader->getUniformLocation("color");
		locations.constructed = true;
	}


	shader->sendUniform4f(locations.material_ambient, ambient);
	shader->sendUniform4f(locations.material_diffuse, diffuse);
	shader->sendUniform4f(locations.material_specular, specular);
	shader->sendUniform1i(locations.material_shininess, shininess);

	if (texture_diffuse.GetName() != "none")
		shader->sendUniform1i(locations.use_texture, 1);
	//else
	//	shader->sendUniform1i(shader->getUniformLocation("use_texture"), 1);

	
	if (reflection)
		shader->sendUniform1i(locations.use_reflection, 1);
	//else
	//	shader->sendUniform1i(shader->getUniformLocation("use_reflection"), 0);

	if (refraction)
		shader->sendUniform1i(locations.use_refraction, 1);
	//else
	//	shader->sendUniform1i(shader->getUniformLocation("use_refraction"), 0);

	/*if (strcmp(texture_bump.GetName().c_str(), "none") == 0)
		shader->sendUniform1i(shader->getUniformLocation("use_bump_texture"), 0);
	else
		shader->sendUniform1i(shader->getUniformLocation("use_bump_texture"), 1);

		*/

	if (emissive)
		shader->sendUniform1i(locations.emissive, 1);
	//else
	//	shader->sendUniform1i(shader->getUniformLocation("emissive"), 0);

}
