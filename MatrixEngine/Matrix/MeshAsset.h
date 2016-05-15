#ifndef MESH_H
#define MESH_H

#include "matrix_engine_internal.h"

#include "Material.h"
#include "VAO.h"

using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Graphics;
using namespace MatrixEngine::Graphics::Components;
using namespace MatrixEngine::Graphics::Components::Materials;

using namespace glm;

using namespace Assimp;

class MeshSection  {
	public :
		MeshSection();
		std::vector<float> vert;
		std::vector<float> norm;
		std::vector<float> tex_coord;
		std::vector<float> tang;
		std::vector<int> indices;

		Material material;
		std::shared_ptr<Memory::VAO> vao;

		void ConstructVAO();
		void Render();
};

class Scene::Components::MeshAsset {
	public :
		MeshAsset();
		~MeshAsset();

		void LoadAsset(std::string file);

	protected :
		std::vector< std::shared_ptr<MeshSection> > sections;
		
		friend class MatrixEngine::Scene::Components::MeshRenderer;

	private :
		void processMesh(aiMesh *ai_mesh, const aiScene *scene);
};
 
#endif
