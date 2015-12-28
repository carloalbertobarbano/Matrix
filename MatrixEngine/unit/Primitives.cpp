#include "Primitives.h"

float triangle_vert[9] =  {  -1.0f, -1.0f, 0.0f,
							  1.0f, -1.0f, 0.0f,
							  0.0f,  1.0f, 0.0f, };

float triangle_color[9] = {  1.0f, 0.0f, 0.0f,
							 0.0f, 1.0f, 0.0f,
							 0.0f, 0.0f, 1.0f  };

int triangle_indices[3] = { 0, 1, 2 };


void DrawTriangle()
{
	static bool init = false;
	static Graphics::Memory::VAO triangleVao;

	if (!init) {
		SDL_Log("Primitives.cpp:DrawTriangle() Initializing triangle primitive\n");

		triangleVao.Construct(1);
		triangleVao.BufferData(0, GL_ARRAY_BUFFER, sizeof(triangle_vert), &triangle_vert[0], GL_STATIC_DRAW);
		triangleVao.VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		triangleVao.BufferData(1, GL_ARRAY_BUFFER, sizeof(triangle_color), &triangle_color[0], GL_STATIC_DRAW);
		triangleVao.VertexAttribute(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		triangleVao.Unbind();
		init = true;
	}

	
	//triangleVao.VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	triangleVao.DrawArrays(GL_TRIANGLES, 0, 3);
}    


float cube_vert[] = { -1.0,-1.0,0.0, -1.0,1.0,0.0, 1.0,1.0,0.0, 1.0, 1.0, 0.0, 1.0,-1.0,0.0, -1.0, -1.0, 0.0 };
float cube_color[] = {  1.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 1.0f,

						1.0f, 0.0f, 1.0f,
						1.0f, 0.0f, 1.0f,
						0.0f, 1.0f, 1.0f };

void DrawCube()
{
	static bool init = false;
	static Graphics::Memory::VAO cubeVao;

	if (!init) {
		std::cout << "Primitives.cpp:DrawTriangle() Initializing Cube primitive" << std::endl;
		
		cubeVao.Construct(1);
		cubeVao.BufferData(0, GL_ARRAY_BUFFER, sizeof(cube_vert), &cube_vert[0], GL_STATIC_DRAW);
		cubeVao.VertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		cubeVao.BufferData(1, GL_ARRAY_BUFFER, sizeof(cube_color), &cube_color[0], GL_STATIC_DRAW);
		cubeVao.VertexAttribute(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		cubeVao.Unbind();
		init = true;
	}

	cubeVao.DrawArrays(GL_TRIANGLES, 0, 6);
}