#ifndef UNIT_FRAME_BUFFER_TEST_H
#define UNIT_FRAME_BUFFER_TEST_H

#include "../Commons.h"
#include "../Matrix/Matrix.h"

#include "KeyboardListener.h"
#include "MouseListener.h"

#include "Primitives.h"

using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Graphics;
using namespace MatrixEngine::Graphics::Components;

void RunUnitFrameBufferTest()
{
    Core::_pCurrentDevice->SetCaption("Matrix Engine - Frame Buffer Test");

	std::shared_ptr<FrameBuffer> fbo(new FrameBuffer(FrameBuffer::RENDER_BUFFER, vec2(1920, 1080)));
	fbo->LoadShader("data/shader/fbo_shader.vs", "data/shader/fbo_shader.fs");

	std::shared_ptr<ShaderProgram> shaderPassthrough(new ShaderProgram());
	std::shared_ptr<ShaderSource>  shaderPassthroughVertex(new ShaderSource());
	std::shared_ptr<ShaderSource>  shaderPassthroughFragment(new ShaderSource());

	shaderPassthroughVertex->LoadFromFile(GL_VERTEX_SHADER, "data/shader/passthrough.vs");
	shaderPassthroughFragment->LoadFromFile(GL_FRAGMENT_SHADER, "data/shader/passthrough.fs");

	shaderPassthrough->AttachShader(shaderPassthroughVertex.get());
	shaderPassthrough->AttachShader(shaderPassthroughFragment.get());
	shaderPassthrough->Compile();

    glm::vec2 res = Core::_pCurrentDevice->GetResolution();
	glm::vec3 cameraPos = vec3(0.0, 0.0, 10.0);
	glm::mat4 projMat = glm::perspective(glm::radians(60.0f), (float)( res.x / res.y), 0.1f, 100.0f);
	glm::mat4 viewMat = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 modMat = glm::mat4(1.0f);


	//modMat = glm::translate(modMat, vec3(0.0, 0.0, -5.0));


	//RenderPipeline::DisableDepth();

	float time = 0.0;

	while (running) {
		Core::_pCurrentDevice->HandleInput();

		RenderPipeline::ClearScreen(vec4(0.0, 0.0, 0.5, 1.0));
		RenderPipeline::PrepareForRendering();

		shaderPassthrough->sendUniformMatrix4fv(shaderPassthrough->getUniformLocation("projMat"), 1, GL_FALSE, projMat);
		shaderPassthrough->sendUniformMatrix4fv(shaderPassthrough->getUniformLocation("viewMat"), 1, GL_FALSE, viewMat);
		shaderPassthrough->sendUniformMatrix4fv(shaderPassthrough->getUniformLocation("modMat"), 1, GL_FALSE, modMat);

		fbo->shader->sendUniform1f(fbo->shader->getUniformLocation("time"), time);
        fbo->Bind();
		RenderPipeline::ClearScreen(vec4(0.0, 0.0, 0.0, 1.0));
		shaderPassthrough->bind();
        DrawTriangle();
		shaderPassthrough->unbind();
		fbo->Unbind();

		RenderPipeline::DisableDepth();
		fbo->Render(vec4(res.x/2 - 500, res.y/2 - 300, 1000, 600));
		RenderPipeline::EnableDepth();

		cameraPos = vec3(5*cos(time), 1.0, 5*sin(time));
		viewMat = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		time += 0.01;

		RenderPipeline::SwapBuffer();
	}

	shaderPassthrough->DetachShader(shaderPassthroughVertex.get());
	shaderPassthrough->DetachShader(shaderPassthroughFragment.get());

	running = true;
}
#endif
