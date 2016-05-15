#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include "matrix_engine_internal.h"

using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Core::IO;

class Scene::SceneEntity {
	public :
		SceneEntity(std::string name = "");
		~SceneEntity();

		void AddChild(SceneEntity *child);
		void RemoveChild(SceneEntity *child);

		Scene::SceneEntity *GetParent();
		Scene::SceneEntity *GetChild(int index);
		int GetChildrenCount();

		void SetNodeName(std::string name);
		std::string GetNodeName();

		void Translate(glm::vec3 p);
		void Rotate(float angle, float x, float y, float z);
		void Scale(glm::vec3 s);
		void BuildModelMatrix();

		glm::vec3 GetPosition() { return pos; }
		glm::vec4 GetRotation() { return glm::vec4(angle, rot.x, rot.y, rot.z); }
		glm::vec3 GetScale() { return scale; }
		glm::mat4 GetModelMatrix() { return modMatrix; }

		void RenderThis();
		void RenderThisAndSub();
		
		virtual void Render();

	protected :
		std::string node_name;

		SceneEntity *parent;
		std::vector<SceneEntity*> children;

		glm::vec3 pos;
		glm::vec3 rot;
		float angle;
		glm::vec3 scale;

		glm::mat4 modMatrix;

		void SetParent(SceneEntity *parent);

};
#endif
