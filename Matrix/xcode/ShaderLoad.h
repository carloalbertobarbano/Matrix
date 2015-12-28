#ifndef UNIT_SHADER_LOAD_H
#define UNIT_SHADER_LOAD_H

#include "../Matrix/Matrix.h"

#include "../Commons.h"
#include "KeyboardListener.h"
#include "MouseListener.h"


using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Graphics;
using namespace MatrixEngine::Graphics::Shader;

void RunUnitShaderLoad()
{
	ShaderProgram *program = new ShaderProgram();
	ShaderSource *vertex = new ShaderSource();
	ShaderSource *fragment = new ShaderSource();

	vertex->LoadFromFile(GL_VERTEX_SHADER, "data/shader/fbo_shader.vs");
	fragment->LoadFromFile(GL_FRAGMENT_SHADER, "data/shader/fbo_shader.fs");
	program->AttachShader(vertex);
	program->AttachShader(fragment);
	program->Compile();

	std::cout << "Binding.." << std::endl;
	program->bind();

	std::cout << "Unbinding.." << std::endl;
	program->unbind();


	program->DetachShader(fragment);
	program->DetachShader(vertex);
	
	delete fragment;
	delete vertex;
	delete program;
}

#endif
