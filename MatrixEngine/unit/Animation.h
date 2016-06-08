#ifndef UNIT_ANIMATION_H
#define UNIT_ANIMATION_H

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
using namespace MatrixEngine::Scene::Components;

void RunUnitAnimation()
{
	Core::_pCurrentDevice->SetCaption("Matrix Engine - Animation Test");

	std::shared_ptr<MeshAsset> meshAsset(new MeshAsset());
	meshAsset->LoadAsset("data/models/animation.dae");

	std::shared_ptr<MeshRenderer> meshRenderer(new MeshRenderer());
	meshRenderer->SetMeshAsset(meshAsset);
	meshRenderer->LoadShader("data/shader/basic.vs", "data/shader/basic.fs");
	meshRenderer->Translate(vec3(0.0, 0.0, 10.0));

	std::shared_ptr<SceneGraph> sceneGraph(new SceneGraph());
	sceneGraph->AddEntityNode(meshRenderer.get());

	std::shared_ptr<Scene::Components::Camera> camera(new Scene::Components::Camera());
	vec3 cameraPos = vec3(0.0, 0.0, 0.0);
	MouseListener::updateCamera = true;
	float time = 0.0;

	while (running) {
		Core::_pCurrentDevice->HandleInput();

		RenderPipeline::ClearScreen(vec4(0.10, 0.10, 0.10, 1.0));
		RenderPipeline::PrepareForRendering();

		camera->ApplyTransform();

		meshRenderer->Rotate(time, 0.0, 1.0, 0.0);

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
