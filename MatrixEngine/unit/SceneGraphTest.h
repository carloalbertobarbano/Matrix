#ifndef UNIT_SCENE_GRAPH_TEST_H
#define UNIT_SCENE_GRAPH_TEST_H


#include "../Commons.h"
#include "../Matrix/Matrix.h"

#include "KeyboardListener.h"
#include "MouseListener.h"

#include <vector>
#include <ctime>
#include <chrono>

using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Graphics;
using namespace MatrixEngine::Graphics::Components;

void RunUnitSceneGraph()
{
	Core::_pCurrentDevice->SetCaption("Matrix Engine - SceneGraphTest");

	
	std::shared_ptr<Scene::Components::MeshAsset> meshAssetSidious(new Scene::Components::MeshAsset());
	meshAssetSidious->LoadAsset("data/models/maul.obj");
	
	std::shared_ptr<Scene::Components::MeshRenderer> meshRendererSidious(new Scene::Components::MeshRenderer());
	meshRendererSidious->SetMeshAsset(meshAssetSidious);
	meshRendererSidious->LoadShader("data/shader/basic.vs", "data/shader/basic.fs");
	meshRendererSidious->Translate(vec3(0.0, -2.0, 0.0));

	std::shared_ptr<Scene::Components::MeshAsset> meshAssetSaber(new Scene::Components::MeshAsset());
	meshAssetSaber->LoadAsset("data/models/lightsaber.obj");
	
	std::shared_ptr<Scene::Components::MeshRenderer> meshRendererSaber(new Scene::Components::MeshRenderer());

	meshRendererSaber->SetMeshAsset(meshAssetSaber);
	meshRendererSaber->LoadShader("data/shader/basic.vs", "data/shader/basic.fs");
	meshRendererSaber->Translate(vec3(-1.0, 3.0, -0.5));
	meshRendererSaber->Rotate(10.0, 1.0, 0.0, 0.0);
	meshRendererSidious->AddChild(meshRendererSaber.get());

	SDL_Log("meshRendererSidious children count: %d", meshRendererSidious->GetChildrenCount());

	std::shared_ptr<Scene::SceneGraph> sceneGraph(new Scene::SceneGraph());
	sceneGraph->AddEntityNode(meshRendererSidious.get());

	glm::vec2 res = Core::_pCurrentDevice->GetResolution();
	glm::vec3 cameraPos = vec3(0.0, 2.0, 10.0);

	float time = 0.0;

	while (running) {
		Core::_pCurrentDevice->HandleInput();

		RenderPipeline::ClearScreen(vec4(0.0, 0.0, 0.5, 1.0));
		
		meshRendererSidious->shader->sendUniform1f(meshRendererSidious->shader->getUniformLocation("time"), time);
		meshRendererSidious->Rotate(time * 1, 0.0, 1.0, 0.0);

		sceneGraph->RenderScene();

		RenderPipeline::SwapBuffer();

		//cameraPos.x = sin(time) * 7; // = vec3(5 * cos(time), 2.0, 5 * sin(time));
		//cameraPos.z = cos(time) * 7 + 3;
		RenderPipeline::glViewMatrix = glm::lookAt(cameraPos, glm::vec3(0.0, 1.5, 0.0), glm::vec3(0.0, 1.0, 0.0));
		time = SDL_GetTicks() * 0.1;
	}

	running = true;
}
#endif