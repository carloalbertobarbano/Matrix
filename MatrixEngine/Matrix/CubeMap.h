#include "matrix_engine_internal.h"

#include "Material.h"
#include "VAO.h"
#include "SceneEntity.h"
#include "TextureCube.h"

using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Graphics;
using namespace MatrixEngine::Graphics::Components;
using namespace MatrixEngine::Graphics::Components::Materials;

class Scene::Components::CubeMap : public Scene::SceneEntity{
	public :
		CubeMap();
		~CubeMap();

		void LoadTextureCube(std::string *files);
		TextureCube *GetTextureCube() { return textureCube.get(); }

		void Render();
	protected :
		std::shared_ptr<Graphics::Components::Textures::TextureCube> textureCube;

		static std::shared_ptr<Shader::ShaderProgram> shader;
		static std::shared_ptr<Shader::ShaderSource>  shader_vert, shader_frag;

		static float vertices[];
		static bool init;
		static Graphics::Memory::VAO vao;
};
