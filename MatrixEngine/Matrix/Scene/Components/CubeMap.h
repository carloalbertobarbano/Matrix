#include "../../matrix_engine_internal.h"
#include "../../Graphics/Components/Materials/Material.h"
#include "../../Graphics/Components/Textures/TextureCube.h"
#include "../../Graphics/Memory/VAO.h"
#include "../../Graphics/Shaders/Shader.h"
#include "../../Scene/SceneEntity.h"


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

		static std::shared_ptr<Shaders::ShaderProgram> shader;
		static std::shared_ptr<Shaders::ShaderSource>  shader_vert, shader_frag;

		static float vertices[];
		static bool init;
		static Graphics::Memory::VAO vao;
};
