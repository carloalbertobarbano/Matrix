#include "Camera.h"

using namespace MatrixEngine::Scene::Components;

Camera::Camera()
{
	pitch = 0.0;
	yaw = 0.0;

	position = vec3(0.0);
}

void Camera::SetPosition(vec3 pos)
{
	position = pos;
}

void Camera::SetPitch(float p)
{
	pitch = p;
}

void Camera::SetYaw(float y)
{
	yaw = y;
}

vec3 Camera::GetPosition()
{
	return position;
}

float Camera::GetPitch()
{
	return pitch;
}

float Camera::GetYaw()
{
	return yaw;
}

void Camera::MoveStrafe(float dist, float dir)
{
	float rad = (yaw + dir) * PI / 180.0;

	position.x -= sin(rad)*dist;
	position.z += cos(rad)*dist;
}

void Camera::MoveUp(float dist, float dir)
{
	float rad = (pitch + dir) * PI / 180.0;

	position.y += sin(rad)*dist;
}

void Camera::ApplyTransform()
{
	Lock();
	RenderPipeline::glViewMatrix = glm::rotate(RenderPipeline::glViewMatrix, pitch, vec3(1.0, 0.0, 0.0));
	RenderPipeline::glViewMatrix = glm::rotate(RenderPipeline::glViewMatrix, yaw, vec3(0.0, 1.0, 0.0));
	RenderPipeline::glViewMatrix = glm::translate(RenderPipeline::glViewMatrix, position);
	
	glViewMatrixNotRotated = mat4(1); // LoadIdentity();
	glViewMatrixNotRotated = glm::translate(glViewMatrixNotRotated, position);

	RenderPipeline::_pCurrentCamera = this;
}

void Camera::Lock()
{
	if (pitch > 90.0)pitch = 90.0;
	if (pitch < -90.0)pitch = -90.0;

	if (yaw < 0.0)yaw += 360.0;
	if (yaw > 360)yaw -= 360.0;
}
