#include "RenderPipeline.h"

using namespace MatrixEngine::Graphics;

mat4 RenderPipeline::glProjectionMatrix;
mat4 RenderPipeline::glViewMatrix;

int  RenderPipeline::RenderMode = RENDER_ALL;
int  RenderPipeline::FillMode = FILL_SOLID;
int  RenderPipeline::ShadeMode = LIGHTING;
vec4 RenderPipeline::color = vec4(1.0);

bool RenderPipeline::constructed = false;

Scene::Components::Camera *RenderPipeline::_pCurrentCamera = NULL;
Graphics::Components::Textures::TextureCube *RenderPipeline::_pCurrentTextureCube = NULL;
Scene::Components::CubeMap *RenderPipeline::_pCurrentCubeMap = NULL;

bool MatrixEngine::Graphics::RenderPipeline::Construct()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);

	glProjectionMatrix = glm::mat4();
	glViewMatrix = glm::mat4();

	constructed = true;
	return true;
}

void MatrixEngine::Graphics::RenderPipeline::ClearScreen(vec4 color)
{
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(color.x, color.y, color.z, color.w);
}

void MatrixEngine::Graphics::RenderPipeline::SetProjectionMatrix(int view_angle, float near_plane, float far_plane, float ratio)
{
	if (ratio == -1)
		ratio = Core::_pCurrentDevice->GetResolution().x / Core::_pCurrentDevice->GetResolution().y;

	glProjectionMatrix = glm::perspective(glm::radians((float)view_angle), ratio, near_plane, far_plane);
}

void MatrixEngine::Graphics::RenderPipeline::ResetViewMatrix()
{
	glViewMatrix = glm::mat4();
}

void MatrixEngine::Graphics::RenderPipeline::PrepareForRendering()
{
	if (!constructed)
		Log::WriteOnStream("RenderPipeline::PrepareForRendering(): Warning, Rendering pipeline not constructed!\n", Log::error_stream);

	SetViewport();
	SetProjectionMatrix();
	ResetViewMatrix();
}
