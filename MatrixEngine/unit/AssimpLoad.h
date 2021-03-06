#ifndef UNIT_ASSIMP_LOAD_H
#define UNIT_ASSIMP_LOAD_H

#include "../Commons.h"
#include "../Matrix/Matrix.h"

#include "KeyboardListener.h"
#include "MouseListener.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <ctime>
#include <chrono>

using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Graphics;
using namespace MatrixEngine::Graphics::Components;
using namespace MatrixEngine::Scene::Components;

using namespace Assimp;

typedef struct {
	std::vector<float> vert;
	std::vector<float> norm;
	std::vector<float> tex_coord;
	std::vector<float> tang;
	std::vector<int> indices;

	std::shared_ptr<Memory::VAO> vao;
	Texture texture;

	void ConstructVAO() {
		int vertex_location = 0;
		int normal_location = 1;
		int texture_location = 2;
		int tangent_location = 3;

		vao = std::make_shared<Memory::VAO>();

		vao->Construct(4);

		vao->BufferData(vertex_location, GL_ARRAY_BUFFER, vert.size()*sizeof(float), &vert.data()[0], GL_STATIC_DRAW);
		vao->VertexAttribute(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		vao->vert_count = vert.size();

		if (norm.size() > 0)
		{
			vao->BufferData(normal_location, GL_ARRAY_BUFFER, norm.size()*sizeof(float), &norm.data()[0], GL_STATIC_DRAW);
			vao->VertexAttribute(normal_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}


		if (tex_coord.size() > 0)
		{
			vao->BufferData(texture_location, GL_ARRAY_BUFFER, tex_coord.size()*sizeof(float), &tex_coord.data()[0], GL_STATIC_DRAW);
			vao->VertexAttribute(texture_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (tang.size() > 0)
		{
			vao->BufferData(tangent_location, GL_ARRAY_BUFFER, tang.size()*sizeof(float), &tang.data()[0], GL_STATIC_DRAW);
			vao->VertexAttribute(tangent_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}


		vao->indices = indices;

		vao->Unbind();
	}

	void Render() {
		texture.BindUnit(0);
		vao->DrawArrays(GL_TRIANGLES, 0, vert.size() / 3);
		texture.Unbind();
	}

} _MeshSection;

typedef struct {
	std::vector< std::shared_ptr<_MeshSection> > section;

	void Render() {
		for (int i = 0; i < section.size(); i++) {
			section[i]->Render();
		}
	}

} _Mesh;

std::shared_ptr<_Mesh> LoadMesh(std::string file);
void processMesh(aiMesh *ai_mesh, const aiScene *scene, std::shared_ptr<_Mesh> mesh);


void RunUnitAssimpLoad()
{
	Core::_pCurrentDevice->SetCaption("Matrix Engine - Assimp Load Test");

	std::shared_ptr<CubeMap> cubemap(new CubeMap());
	std::string maps[6];
	maps[0] = "data/textures/alpine/alpine_left.jpg";
	maps[1] = "data/textures/alpine/alpine_front.jpg";
	maps[2] = "data/textures/alpine/alpine_right.jpg";
	maps[3] = "data/textures/alpine/alpine_back.jpg";
	maps[4] = "data/textures/alpine/alpine_top.jpg";
	maps[5] = "data/textures/alpine/alpine_top.jpg";
	cubemap->LoadTextureCube(maps);

	std::shared_ptr<_Mesh> mesh = LoadMesh("data/models/Stormtrooper.obj");
	SDL_Log("Mesh section: %d\n", mesh->section.size());

	std::shared_ptr<ShaderProgram> shaderPassthrough(new ShaderProgram());
	std::shared_ptr<ShaderSource>  shaderPassthroughVertex(new ShaderSource());
	std::shared_ptr<ShaderSource>  shaderPassthroughFragment(new ShaderSource());

	shaderPassthroughVertex->LoadFromFile(GL_VERTEX_SHADER, "data/shader/basic.vs");
	shaderPassthroughFragment->LoadFromFile(GL_FRAGMENT_SHADER, "data/shader/basic_no_mat.fs");

	shaderPassthrough->AttachShader(shaderPassthroughVertex.get());
	shaderPassthrough->AttachShader(shaderPassthroughFragment.get());
	shaderPassthrough->Compile();

	glm::vec2 res = Core::_pCurrentDevice->GetResolution();
	glm::vec3 cameraPos = vec3(0.0, 2.0, 5.0);
	glm::mat4 projMat = glm::perspective(glm::radians(60.0f), (float)(res.x / res.y), 0.1f, 500.0f);
	glm::mat4 viewMat = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 modMat = glm::scale(glm::mat4(1.0f), vec3(0.021));
	modMat = glm::translate(modMat, vec3(0.0, -2.0, 0.0));

	float time = 0.0;

	std::clock_t startcputime = std::clock();
	int frames = 0;
	while (running) {
		Core::_pCurrentDevice->HandleInput();

		RenderPipeline::ClearScreen(vec4(0.0, 0.0, 0.5, 1.0));
		RenderPipeline::PrepareForRendering();

		cubemap->Render();

		shaderPassthrough->sendUniformMatrix4fv(shaderPassthrough->getUniformLocation("projMat"), 1, GL_FALSE, projMat);
		shaderPassthrough->sendUniformMatrix4fv(shaderPassthrough->getUniformLocation("viewMat"), 1, GL_FALSE, viewMat);
		//shaderPassthrough->sendUniformMatrix4fv(shaderPassthrough->getUniformLocation("modMat"), 1, GL_FALSE, modMat);
		shaderPassthrough->sendUniform1i(shaderPassthrough->getUniformLocation("diffuse_texture"), 0);
		//shaderPassthrough->sendUniform1f(shaderPassthrough->getUniformLocation("time"), time);

		for (int x = -5; x < 5; x++) {
			for (int z = 0; z < 10; z++) {
				modMat = glm::translate(mat4(1.0), vec3(x*2.5, -2.0, -z * 2));
				//modMat2 = glm::rotate(modMat2, time, vec3(0.0, 1.0, 0.0));
				modMat = glm::scale(modMat, vec3(1));
				shaderPassthrough->sendUniformMatrix4fv(shaderPassthrough->getUniformLocation("modMat"), 1, GL_FALSE, modMat);
				shaderPassthrough->bind();
				mesh->Render();
				shaderPassthrough->unbind();
			}
		}

		cameraPos.x = sin(time) * 5; // = vec3(5 * cos(time), 2.0, 5 * sin(time));
		cameraPos.z = cos(time) * 15 - 10;
		viewMat = glm::lookAt(cameraPos, glm::vec3(0.0, 1.5, 0.0), glm::vec3(0.0, 1.0, 0.0));
		time += 0.01;

		RenderPipeline::SwapBuffer();

		frames++;
		double cpu_duration = (std::clock() - startcputime) / (double)CLOCKS_PER_SEC;

		if (cpu_duration >= 1.0) {
			Core::_pCurrentDevice->SetCaption("Matrix Engine - Assimp Load Test - FPS: " + std::to_string(frames));
			frames = 0;
			startcputime = std::clock();
		}
	}

	shaderPassthrough->DetachShader(shaderPassthroughVertex.get());
	shaderPassthrough->DetachShader(shaderPassthroughFragment.get());

	running = true;

	std::shared_ptr<Scene::Components::MeshAsset> asset(new Scene::Components::MeshAsset());
	std::vector< std::shared_ptr<Scene::Components::MeshRenderer> > renderer;
	
	asset->LoadAsset("data/models/Stormtrooper.obj");

	for (int x = -5; x < 5; x++)
		for (int z = 0; z < 10; z++) {
			renderer.push_back(std::shared_ptr<Scene::Components::MeshRenderer>(new Scene::Components::MeshRenderer()));
			renderer.back()->SetMeshAsset(asset);
			renderer.back()->LoadShader("data/shader/basic.vs", "data/shader/basic.fs");
		}	

	cameraPos = vec3(0.0, 2.0, 5.0);
	RenderPipeline::glProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)(res.x / res.y), 0.1f, 500.0f);
	RenderPipeline::glViewMatrix = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));

	while (running) {
		Core::_pCurrentDevice->HandleInput();

		RenderPipeline::ClearScreen(vec4(0.0, 0.0, 0.5, 1.0));
		//RenderPipeline::PrepareForRendering();

		cubemap->Render();

		int i = 0;
		for (int x = -5; x < 5; x++)
			for (int z = 0; z < 10; z++) {
				renderer[i]->Translate(vec3(x*2.5, -2.0, -z * 2));
				renderer[i]->Rotate(time * 0.5, 0.0, 1.0, 0.0);
				renderer[i++]->Render();
			}

		RenderPipeline::SwapBuffer();

		//cameraPos.x = sin(time) * 5;
		//cameraPos.z = cos(time) * 15 - 10;
		//RenderPipeline::glViewMatrix = glm::lookAt(cameraPos, glm::vec3(0.0, 1.5, 0.0), glm::vec3(0.0, 1.0, 0.0));
		//meshRenderer->Rotate(time * 0.5, 0.0, 1.0, 0.0);
		time = SDL_GetTicks() * 0.1;
	}

	running = true;

	std::shared_ptr<Scene::Components::MeshAsset> meshAsset(new Scene::Components::MeshAsset());
	std::shared_ptr<Scene::Components::MeshRenderer> meshRenderer(new Scene::Components::MeshRenderer());

	meshAsset->LoadAsset("data/models/Stormtrooper.obj");
	meshRenderer->SetMeshAsset(meshAsset);
	meshRenderer->LoadShader("data/shader/basic.vs", "data/shader/basic.fs");

	cameraPos = vec3(0.0, 2.0, 5.0);
	RenderPipeline::glProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)(res.x / res.y), 0.1f, 500.0f);
	RenderPipeline::glViewMatrix = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
	meshRenderer->Translate(vec3(0.0, -2.0, 0.0));

	while (running) {
		Core::_pCurrentDevice->HandleInput();

		RenderPipeline::ClearScreen(vec4(0.0, 0.0, 0.5, 1.0));
		//RenderPipeline::PrepareForRendering();
		
		cubemap->Render();
		meshRenderer->Render();

		RenderPipeline::SwapBuffer();

		//cameraPos.x = sin(time) * 5;
		//cameraPos.z = cos(time) * 15 - 10;
		//RenderPipeline::glViewMatrix = glm::lookAt(cameraPos, glm::vec3(0.0, 1.5, 0.0), glm::vec3(0.0, 1.0, 0.0));
		meshRenderer->Rotate(time * 0.5, 0.0, 1.0, 0.0);
		time = SDL_GetTicks() * 0.1;

	}

	running = true;
}


std::shared_ptr<_Mesh> LoadMesh(std::string file)
{
	std::shared_ptr<_Mesh> mesh(new _Mesh());

	SDL_Log("Loading %s\n", file.c_str());

	Importer importer;
	const aiScene *scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		SDL_Log("Could not load file %s\n", file.c_str());
		return nullptr;
	}

	for (int i = 0; i < scene->mNumMeshes; i++) {
		processMesh(scene->mMeshes[i], scene, mesh);
	}

	SDL_Log("Mesh %s loaded\n", file.c_str());

	return mesh;
}

void processMesh(aiMesh *ai_mesh, const aiScene *scene, std::shared_ptr<_Mesh> mesh)
{
	std::shared_ptr<_MeshSection> section(new _MeshSection);
	for (int i = 0; i < ai_mesh->mNumVertices; i++) {
		vec3 m_vert(ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z);
		vec3 m_norm(ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z);
		vec3 m_tang = vec3(0.0); // (mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		vec2 m_tex_coord = vec2(ai_mesh->mTextureCoords[0][i].x, ai_mesh->mTextureCoords[0][i].y);

		section->vert.push_back(m_vert.x);
		section->vert.push_back(m_vert.y);
		section->vert.push_back(m_vert.z);

		section->norm.push_back(m_norm.x);
		section->norm.push_back(m_norm.y);
		section->norm.push_back(m_norm.z);

		section->tang.push_back(m_tang.x);
		section->tang.push_back(m_tang.y);
		section->tang.push_back(m_tang.z);

		section->tex_coord.push_back(m_tex_coord.x);
		section->tex_coord.push_back(-m_tex_coord.y);
	}

	for (int i = 0; i < ai_mesh->mNumFaces; i++) {
		aiFace &face = ai_mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++) {
			section->indices.push_back(face.mIndices[j]);
		}
	}

	aiString path;
	scene->mMaterials[ai_mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL);
	section->texture.LoadFromFile("data/models/" + std::string(path.data));

	SDL_Log("Section %d has %d vertices, %d normals, %d texture coords, %d tangents, %d indices\n", mesh->section.size(), section->vert.size() / 3,
		section->norm.size() / 3,
		section->tex_coord.size() / 2,
		section->tang.size() / 3,
		section->indices.size());
	section->ConstructVAO();
	mesh->section.push_back(section);
}


#endif