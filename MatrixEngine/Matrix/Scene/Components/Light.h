#ifndef LIGHT_H
#define LIGHT_H

#include "../../matrix_engine_internal.h"
#include "../../Scene/SceneEntity.h"

using namespace glm;
using namespace MatrixEngine;


class Scene::Components::Light : public Scene::SceneEntity {
	public :
		Light();
		Light(vec4 ambient, vec4 diffuse, vec4 specular);
		~Light() { }
		
		void SetAmbient(vec4 ambient) { ambient_color = ambient; }
		void SetDiffuse(vec4 diffuse) { diffuse_color = diffuse; }
		void SetSpecular(vec4 specular) { specular_color = specular; }

		vec4 GetAmbient() { return ambient_color;  }
		vec4 GetDiffuse() { return diffuse_color;  }
		vec4 GetSpecular() { return specular_color;  }

		virtual void Update() { };
		virtual void Render();

	protected :
		glm::vec4 ambient_color;
		glm::vec4 diffuse_color;
		glm::vec4 specular_color;
};
 
#endif