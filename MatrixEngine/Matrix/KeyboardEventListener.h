#ifndef KEYBOARD_EVENT_LISTENER_H
#define KEYBOARD_EVENT_LISTENER_H

#include "matrix_engine_internal.h"

using namespace MatrixEngine::Core::IO;

class DeviceListener::KeyboardEventListener {
	public :
		virtual void OnKeyPressed(SDL_Scancode key);
		virtual void OnKeyUp(SDL_Scancode key);
};


#endif
