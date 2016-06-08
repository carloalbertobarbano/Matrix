#include "MouseEventListener.h"

void DeviceListener::MouseEventListener::OnMouseMove(glm::vec2 pos)
{

}

void DeviceListener::MouseEventListener::OnKeyPressed(int button, int x, int y)
{
	Log::WriteOnStream("MouseEventListener::OnKeyPressed(): warning, using non implemented method in virtual class\n", Log::log_stream);
}

void DeviceListener::MouseEventListener::OnKeyUp(int button, int x, int y)
{

}
