#ifndef UNIT_CAMERA_TEST_H
#define UNIT_CAMERA_TEST_H

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

void RunUnitCameraTest()
{
	Core::_pCurrentDevice->SetCaption("Matrix Engine - First person camera test");

	std::shared_ptr<Scene::Components::MeshAsset> meshAsset(new Scene::Components::MeshAsset());
	meshAsset->LoadAsset("data/models/star_wars_scene.obj");

	std::shared_ptr<Scene::Components::MeshRenderer> meshRenderer(new Scene::Components::MeshRenderer());
	meshRenderer->SetMeshAsset(meshAsset);
	meshRenderer->LoadShader("data/shader/basic.vs", "data/shader/basic.fs");
	meshRenderer->Translate(vec3(0.0, 0.0, 0.0));

	std::shared_ptr<Scene::SceneGraph> sceneGraph(new Scene::SceneGraph());
	sceneGraph->AddEntityNode(meshRenderer.get());

	glm::vec2 res = Core::_pCurrentDevice->GetResolution();
	glm::vec3 cameraPos = vec3(0.0, -12.0, -25.0);
	
	std::shared_ptr<Scene::Components::Camera> camera(new Scene::Components::Camera());
	camera->SetPosition(cameraPos);

	MouseListener::updateCamera = true;

	float time = 0.0;

	while (running) {
		Core::_pCurrentDevice->HandleInput();


		RenderPipeline::ClearScreen(vec4(0.0, 0.0, 0.0, 1.0));
		RenderPipeline::PrepareForRendering();
		  
		camera->ApplyTransform();

		meshRenderer->shader->sendUniform1f(meshRenderer->shader->getUniformLocation("time"), time);
		
		sceneGraph->RenderScene();

		RenderPipeline::SwapBuffer();

		
		time = SDL_GetTicks() * 0.1;

		if (KeyboardListener::keyPressed[SDL_SCANCODE_C]) {
			cameraPos = camera->GetPosition();
			cameraPos.x = sin(time * 0.01) * 5;
			cameraPos.z = cos(time * 0.01) * 15 - 10;
			camera->SetPosition(cameraPos);

			camera->LookAt(glm::vec3(0.0, 1.5, 0.0), glm::vec3(0.0, 1.0, 0.0));
		}


		if (KeyboardListener::keyPressed[SDL_SCANCODE_W]) {
			camera->MoveStrafe(0.5, 0.0);
			camera->MoveUp(0.5, 0.0);
		}

		if (KeyboardListener::keyPressed[SDL_SCANCODE_S]) {
			camera->MoveStrafe(0.5, 180.0);
			camera->MoveUp(0.5, 180.0);
		}

		if (KeyboardListener::keyPressed[SDL_SCANCODE_A]) {
			camera->MoveStrafe(0.5, -90.0);
		}

		if (KeyboardListener::keyPressed[SDL_SCANCODE_D]) {
			camera->MoveStrafe(0.5, 90.0);
		}
	}

	running = true;
}

#endif