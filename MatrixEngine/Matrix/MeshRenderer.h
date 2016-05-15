#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "matrix_engine_internal.h"
#include "Shader.h"
#include "Camera.h"
#include "SceneEntity.h"
#include "MeshAsset.h"

using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Graphics;
using namespace MatrixEngine::Graphics::Components;
using namespace MatrixEngine::Graphics::Components::Materials;

using namespace glm;

class Scene::Components::MeshRenderer : public Scene::SceneEntity {
	public :
		MeshRenderer();
		MeshRenderer(std::shared_ptr<Scene::Components::MeshAsset> mesh);
		~MeshRenderer();

		void SetMeshAsset(std::shared_ptr<Scene::Components::MeshAsset> mesh);
		void LoadShader(std::string vertex, std::string fragment, std::string geometry = "", std::string tessellation1 = "", std::string tessellation2 = "");

		void Update() { };
		void Render();

		std::shared_ptr<Shader::ShaderProgram> shader;
	protected :
		std::shared_ptr<Shader::ShaderSource> shader_vert, shader_frag, shader_geom, shader_tess1, shader_tess2;

		std::shared_ptr<Scene::Components::MeshAsset> _pMeshAsset;
};
#endif