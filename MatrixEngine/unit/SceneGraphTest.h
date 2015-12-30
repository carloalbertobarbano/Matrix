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
	meshAssetSidious->LoadAsset("data/models/Darth_Sidious.obj");
	
	std::shared_ptr<Scene::Components::MeshRenderer> meshRendererSidious(new Scene::Components::MeshRenderer());
	meshRendererSidious->SetMeshAsset(meshAssetSidious);
	meshRendererSidious->LoadShader("data/shader/basic.vs", "data/shader/basic.fs");
	meshRendererSidious->Translate(vec3(0.0, -2.0, 0.0));

	std::shared_ptr<Scene::Components::MeshAsset> meshAssetRock(new Scene::Components::MeshAsset());
	meshAssetRock->LoadAsset("data/models/Rock.obj");
	
	std::vector<std::shared_ptr<Scene::Components::MeshRenderer> > meshRendererRock; // (new Scene::Components::MeshRenderer());

	for (int i = 0; i < 10; i++) {
		vec3 random((rand() % 20) - 9, rand() % 5 , rand() % 10 - 5);

		meshRendererRock.push_back(std::make_shared<Scene::Components::MeshRenderer>());
		meshRendererRock[i]->SetMeshAsset(meshAssetRock);
		meshRendererRock[i]->LoadShader("data/shader/basic.vs", "data/shader/basic.fs");
		meshRendererRock[i]->Translate(random);
		meshRendererRock[i]->Scale(vec3(0.8, 0.8, 0.8) + random/20.0f);
		meshRendererSidious->AddChild(meshRendererRock[i].get());
	}

	SDL_Log("meshRendererSidious children count: %d", meshRendererSidious->GetChildrenCount());

	std::shared_ptr<Scene::SceneGraph> sceneGraph(new Scene::SceneGraph());
	sceneGraph->AddEntityNode(meshRendererSidious.get());

	glm::vec2 res = Core::_pCurrentDevice->GetResolution();
	glm::vec3 cameraPos = vec3(0.0, 2.0, 10.0);

	float time = 0.0;

	while (running) {
		Core::_pCurrentDevice->HandleInput();

		RenderPipeline::ClearScreen(vec4(0.0, 0.0, 0.5, 1.0));
		


		meshRendererSidious->Rotate(time * 0.1, 0.0, 1.0, 0.0);
		
		for (int i = 0; i < meshRendererRock.size(); i++) {
			meshRendererRock[i]->Rotate(time * i, 0.0, 1.0, 0.0);
		}
		//meshRendererRock->Rotate(time * 1, 0.0, 1.0, 0.0);

		//meshRendererStormtrooper->Scale(vec3((sin(time) + 2.0) / 2.0, 1.0, 1.0));
		sceneGraph->RenderScene();

		RenderPipeline::SwapBuffer();

		//cameraPos.x = sin(time) * 7; // = vec3(5 * cos(time), 2.0, 5 * sin(time));
		//cameraPos.z = cos(time) * 7 + 3;
		RenderPipeline::glViewMatrix = glm::lookAt(cameraPos, glm::vec3(0.0, 1.5, 0.0), glm::vec3(0.0, 1.0, 0.0));
		time = SDL_GetTicks() * 0.001;
	}

	running = true;
}
#endif