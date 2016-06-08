#ifndef GLXSDL_CAMERA_H
#define GLXSDL_CAMERA_H

#include "../../matrix_engine_internal.h"
#include "../../Graphics/RenderPipeline.h"

#define PI 3.14159265359

using namespace MatrixEngine;
using namespace MatrixEngine::Graphics;
using namespace MatrixEngine::Scene;

class Scene::Components::Camera
{
public:
	Camera();

	void SetPosition(vec3 pos);
	void SetPitch(float p);
	void SetYaw(float y);

	vec3  GetPosition();
	float GetPitch();
	float GetYaw();

	void MoveStrafe(float distance, float direction);
	void MoveUp(float dist, float direction);

	void ApplyTransform();
	
	void LookAt(vec3 &center, vec3 &up);

	mat4 GetViewMatrixNotRotated(){ return glViewMatrixNotRotated; }

protected:
	vec3 position;
	float pitch, yaw;

	void Lock();

	mat4 glViewMatrixNotRotated;
};

#endif