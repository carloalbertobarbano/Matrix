#include "CubeMap.h"
#include "Camera.h"
#include "RenderPipeline.h"

std::shared_ptr<Shader::ShaderProgram> Scene::Components::CubeMap::shader;
std::shared_ptr<Shader::ShaderSource> Scene::Components::CubeMap::shader_vert;
std::shared_ptr<Shader::ShaderSource> Scene::Components::CubeMap::shader_frag;
Graphics::Memory::VAO Scene::Components::CubeMap::vao;


bool Scene::Components::CubeMap::init = false;
float Scene::Components::CubeMap::vertices[] = {
	-10.0f,  10.0f, -10.0f,
	-10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,

	-10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f, -10.0f,  10.0f,

	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,

	-10.0f, -10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f,  10.0f,

	-10.0f,  10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f, -10.0f,

	-10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f,  10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f,  10.0f,
	10.0f, -10.0f,  10.0f
};

Scene::Components::CubeMap::CubeMap()
{
	if (!init) {
		SDL_Log("CubeMap::CubeMap() Initializing cube primitive\n");
		vao.Construct(1);
		vao.BufferData(0, GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
		vao.VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		vao.Unbind();

		SDL_Log("CubeMap::CubeMap() Initializing CubeMap shaders\n");
		shader_vert = std::make_shared<Shader::ShaderSource>();
		shader_frag = std::make_shared<Shader::ShaderSource>();
		shader		= std::make_shared<Shader::ShaderProgram>();

		shader_vert->InitWithSource(GL_VERTEX_SHADER,
			"#version 330\n" \
			"#extension GL_ARB_separate_shader_objects: enable\n"
			"layout(location = 0)in vec3 vertex; " \
			"uniform mat4 projMat; " \
			"uniform mat4 viewMat; " \
			"uniform mat4 modMat; " \
			"out vec3 tex_coord; " \
			"void main(){ tex_coord = vertex; gl_Position = projMat * viewMat * modMat * vec4(vertex, 1.0); } ");

		shader_frag->InitWithSource(GL_FRAGMENT_SHADER,
			"#version 330\n" \
			"#extension GL_ARB_separate_shader_objects: enable\n" \
			"in vec3 tex_coord; " \
			"uniform samplerCube cube_texture; " \
			"out vec4 fragColor; " \
			"void main(){ fragColor = texture(cube_texture, tex_coord); }");

		shader->AttachShader(shader_vert.get());
		shader->AttachShader(shader_frag.get());
		shader->Compile();

		init = true;
	}

	textureCube = nullptr;
	RenderPipeline::_pCurrentCubeMap = this;
}

Scene::Components::CubeMap::~CubeMap()
{
	if (RenderPipeline::_pCurrentCubeMap == this)
		RenderPipeline::_pCurrentCubeMap = NULL;
}

void Scene::Components::CubeMap::LoadTextureCube(std::string * files)
{
	textureCube = std::make_shared<Textures::TextureCube>();
	textureCube->LoadFromFiles(files);
}

void Scene::Components::CubeMap::Render()
{
	RenderPipeline::_pCurrentCubeMap = this;
	this->BuildModelMatrix();
	vec3 pos = (RenderPipeline::_pCurrentCamera ? -RenderPipeline::_pCurrentCamera->GetPosition() : vec3(0.0));
	modMatrix = glm::translate(modMatrix, pos);

	glDepthMask(GL_FALSE);

	shader->sendUniformMatrix4fv(shader->getUniformLocation("projMat"), 1, GL_FALSE, RenderPipeline::glProjectionMatrix);
	shader->sendUniformMatrix4fv(shader->getUniformLocation("viewMat"), 1, GL_FALSE, RenderPipeline::glViewMatrix);
	shader->sendUniformMatrix4fv(shader->getUniformLocation("modMat"), 1, GL_FALSE, modMatrix);
	shader->sendUniform1i(shader->getUniformLocation("cube_texture"), 0);

	textureCube->BindUnit(0);
	shader->bind();
	vao.DrawArrays(GL_TRIANGLES, 0, 36);
	shader->unbind();
	textureCube->UnbindUnit(0);

	glDepthMask(GL_TRUE);
}


