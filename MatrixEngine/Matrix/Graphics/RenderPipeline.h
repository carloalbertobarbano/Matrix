#ifndef RENDER_PIPELINE_H
#define RENDER_PIPELINE_H

#include "../matrix_engine_internal.h"
#include "../Core/Device.h"

using namespace MatrixEngine;
using namespace glm;


class MatrixEngine::Graphics::RenderPipeline
{
public:
	static bool Construct();

	static void ClearScreen(vec4 color = vec4(vec3(0.0), 1.0));

	static void inline SwapBuffer() { SDL_GL_SwapWindow(Core::_pCurrentDevice->GetWindow()); }

	static void inline SetViewport(vec4 view = vec4(0.0, 0.0, Core::_pCurrentDevice->GetResolution().x, Core::_pCurrentDevice->GetResolution().y))
	{
		glViewport(view.x, view.y, view.z, view.w);
	}

	static void SetProjectionMatrix(int view_angle = 60, float near_plane = 0.01, float far_plane = 500, float ratio = -1);
	static void ResetViewMatrix();

	static mat4 glProjectionMatrix;
	static mat4 glViewMatrix;

	static void PrepareForRendering();

	static void inline DisableDepth() { glDisable(GL_DEPTH_TEST); }
	static void inline EnableDepth() { glEnable(GL_DEPTH_TEST); }

	enum RenderFlag { RENDER_ALL, RENDER_SOLID, RENDER_TRANSPARENT };
	enum FillMode { FILL_WIREFRAME, FILL_SOLID };
	enum ShadeMode { LIGHTING, COLOR };

	static int RenderMode;
	static int FillMode;
	static int ShadeMode;
	static vec4 color;

	static float DistanceForBumpMapping;

	static Scene::Components::Camera *_pCurrentCamera;
	static Graphics::Components::Textures::TextureCube *_pCurrentTextureCube;
	static Scene::Components::CubeMap *_pCurrentCubeMap;

protected:
	static bool constructed;
};

#endif