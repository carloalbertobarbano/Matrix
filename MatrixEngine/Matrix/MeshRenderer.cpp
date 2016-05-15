#include "MeshRenderer.h"
#include "CubeMap.h"
#include "TextureCube.h"

Scene::Components::MeshRenderer::MeshRenderer() : SceneEntity()
{
	shader = nullptr;
	shader_vert = nullptr;
	shader_frag = nullptr;
	shader_geom = nullptr;
	shader_tess1 = nullptr;
	shader_tess2 = nullptr;
}

Scene::Components::MeshRenderer::MeshRenderer(std::shared_ptr<Scene::Components::MeshAsset> mesh) : SceneEntity()
{
	SetMeshAsset(mesh);
	shader = nullptr;
	shader_vert = nullptr;
	shader_frag = nullptr;
	shader_geom = nullptr;
	shader_tess1 = nullptr;
	shader_tess2 = nullptr;
}

Scene::Components::MeshRenderer::~MeshRenderer()
{
	if (shader)
	{
		shader->DetachShader(shader_vert.get());
		shader->DetachShader(shader_frag.get());

		if (shader_geom)shader->DetachShader(shader_geom.get());
		if (shader_tess1)shader->DetachShader(shader_tess1.get());
		if (shader_tess2)shader->DetachShader(shader_tess2.get());
	}
}

void Scene::Components::MeshRenderer::SetMeshAsset(std::shared_ptr<Scene::Components::MeshAsset> mesh)
{
	this->_pMeshAsset = mesh;
}

void Scene::Components::MeshRenderer::LoadShader(std::string vertex, std::string fragment, std::string geometry, std::string tessellation1, std::string tessellation2)
{
	shader_vert = std::make_shared<Shader::ShaderSource>();
	shader_frag = std::make_shared<Shader::ShaderSource>();
	if(geometry != "")shader_geom = std::make_shared<Shader::ShaderSource>();
	if(tessellation1 != "")shader_tess1 = std::make_shared<Shader::ShaderSource>();
	if(tessellation2 != "")shader_tess2 = std::make_shared<Shader::ShaderSource>();
	shader = std::make_shared<Shader::ShaderProgram>();

	shader_vert->LoadFromFile(GL_VERTEX_SHADER, vertex);
	shader_frag->LoadFromFile(GL_FRAGMENT_SHADER, fragment);
#ifdef __APPLE__
    if (shader_geom)shader_geom->LoadFromFile(GL_GEOMETRY_SHADER_EXT, geometry);
#else
    if (shader_geom)shader_geom->LoadFromFile(GL_GEOMETRY_SHADER, geometry);
#endif
	/* TODO: Manage tessellation shaders*/

	shader->AttachShader(shader_vert.get());
	if (shader_geom)shader->AttachShader(shader_geom.get());
	shader->AttachShader(shader_frag.get());
	shader->Compile();
}

void Scene::Components::MeshRenderer::Render()
{
	this->BuildModelMatrix();

	if (shader) {
		shader->sendUniformMatrix4fv(shader->getUniformLocation("projMat"), 1, GL_FALSE, RenderPipeline::glProjectionMatrix);
		shader->sendUniformMatrix4fv(shader->getUniformLocation("viewMat"), 1, GL_FALSE, RenderPipeline::glViewMatrix);
		shader->sendUniformMatrix4fv(shader->getUniformLocation("modMat"), 1, GL_FALSE, modMatrix);
		shader->sendUniform1i(shader->getUniformLocation("diffuse_texture"), 0);
		if(RenderPipeline::_pCurrentCubeMap)
			shader->sendUniform1i(shader->getUniformLocation("texture_cube"), 1);
		//shader->sendUniform1i(shader->getUniformLocation("texture_bump"), 2);

		vec3 camera_pos = vec3(0.0);
		if(RenderPipeline::_pCurrentCamera)
			camera_pos = -RenderPipeline::_pCurrentCamera->GetPosition();
		shader->sendUniform3f(shader->getUniformLocation("cameraPosition"), camera_pos);

		if (RenderPipeline::ShadeMode == RenderPipeline::COLOR)
		{
			//shader->sendUniform1i(shader->getUniformLocation("render_all_with_color"), 1);
			//shader->sendUniform4f(shader->getUniformLocation("color"), RenderPipeline::color);
		}
		else
		{
			//shader->sendUniform1i(shader->getUniformLocation("render_all_with_color"), 0);
		}
	}

	for (int i = 0; i < _pMeshAsset->sections.size(); i++) {
		if (shader) {
			if (RenderPipeline::_pCurrentCubeMap)
				RenderPipeline::_pCurrentCubeMap->GetTextureCube()->BindUnit(1);
			_pMeshAsset->sections[i]->material.texture_diffuse.BindUnit(0);
			_pMeshAsset->sections[i]->material.sendMaterialUniforms(shader.get());
			shader->bind();
			
			//_pMeshAsset->sections[i]->material.texture_bump.BindUnit(2);
		}


		_pMeshAsset->sections[i]->Render();

		if (shader) {
			shader->unbind();
			_pMeshAsset->sections[i]->material.texture_diffuse.Unbind();
			_pMeshAsset->sections[i]->material.texture_bump.Unbind();
			if (RenderPipeline::_pCurrentCubeMap)
				RenderPipeline::_pCurrentCubeMap->GetTextureCube()->UnbindUnit(1);
		}
	}
}
