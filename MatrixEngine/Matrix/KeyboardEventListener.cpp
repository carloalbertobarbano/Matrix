#include "KeyboardEventListener.h"

void DeviceListener::KeyboardEventListener::OnKeyPressed(SDL_Scancode key)
{
	Log::WriteOnStream("KeyboardEventListener::OnKeyPressed(): warning, using non implemented method in virtual class\n", Log::log_stream);
}

void DeviceListener::KeyboardEventListener::OnKeyUp(SDL_Scancode key)
{

}
