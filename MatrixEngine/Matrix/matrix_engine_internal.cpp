#include "matrix_engine_internal.h"
#include <stdio.h>

#include "Graphics/Memory/VAO.h"

#if _MSC_VER >= 1400
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }
#endif

using namespace MatrixEngine;
using namespace MatrixEngine::Core;

std::ofstream IO::Log::error_stream;
std::ofstream IO::Log::log_stream;

Core::Device *Core::_pCurrentDevice = NULL;
Scene::SceneGraph *Scene::_pCurrentSceneGraph = NULL;

void MatrixEngine::Core::IO::Log::WriteOnStream(std::string msg, std::ofstream & os) {
	SDL_Log(msg.c_str());
	os << msg;
}

char * MatrixEngine::Core::IO::Log::TextFileRead(std::string filepath)
{
	char *text = NULL;

	if (filepath.c_str() != NULL)
	{
		FILE *file;
#ifndef _WIN32
        	file = fopen(filepath.c_str(), "rt");
#else
		fopen_s(&file, filepath.c_str(), "rt");
#endif
		if (file != NULL) {
			fseek(file, 0, SEEK_END);
			int count = ftell(file);
			rewind(file);

			if (count > 0) {
				text = (char*)malloc(sizeof(char)*(count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);
		}
	}
	return text;
}

/*float triangle_vert[9] = { -1.0f, -1.0f, 0.0f,
							1.0f, -1.0f, 0.0f,
							0.0f,  1.0f, 0.0f, };

int triangle_indices[3] = { 0, 1, 2 };

void MatrixEngine::Graphics::Utils::Primitives::DrawTriangle()
{
	static bool init = false;
	static Graphics::Memory::VAO triangleVao;

	if (!init) {
		SDL_Log("Primitives.cpp:DrawTriangle() Initializing triangle primitive\n");


		triangleVao.Construct(2);
		triangleVao.BufferData(0, GL_ARRAY_BUFFER, sizeof(triangle_vert), &triangle_vert[0], GL_STATIC_DRAW);
		triangleVao.VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		triangleVao.Unbind();
		init = true;

		SDL_Log("Done\n");
	}

	triangleVao.DrawArrays(GL_TRIANGLES, 0, 3);
}

float cube_vert[] = { -1.0,-1.0,0.0, 
					  -1.0,1.0,0.0, 
					   1.0,1.0,0.0, 
	                   1.0, 1.0, 0.0, 
					   1.0,-1.0,0.0, 
		              -1.0, -1.0, 0.0 };

void MatrixEngine::Graphics::Utils::Primitives::DrawCube()
{
	static bool init = false;
	static Graphics::Memory::VAO cubeVao;

	if (!init) {
		std::cout << "Primitives.cpp:DrawTriangle() Initializing Cube primitive" << std::endl;

		cubeVao.Construct(2);
		cubeVao.BufferData(0, GL_ARRAY_BUFFER, sizeof(cube_vert), &cube_vert[0], GL_STATIC_DRAW);
		cubeVao.VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		cubeVao.Unbind();
		init = true;
	}

	cubeVao.DrawArrays(GL_TRIANGLES, 0, 6);
}*/
