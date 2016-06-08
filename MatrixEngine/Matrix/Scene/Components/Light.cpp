#include "Light.h"

Scene::Components::Light::Light()
{
	ambient_color = vec4(0.0, 0.0, 0.0, 1.0);
	diffuse_color = vec4(0.7, 0.7, 0.7, 1.0);
	specular_color = vec4(1.0, 1.0, 1.0, 1.0);
}

Scene::Components::Light::Light(vec4 ambient, vec4 diffuse, vec4 specular)
{
	SetAmbient(ambient);
	SetDiffuse(diffuse);
	SetSpecular(specular);
}

void Scene::Components::Light::Render()
{
	//TODO: primitives methods! (Cubes)
}
