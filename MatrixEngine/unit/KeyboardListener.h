#ifndef UNIT_KEYBOARD_LISTENER_H
#define UNIT_KEYBOARD_LISTENER_H

#include "../Matrix/Matrix.h"
#include "../Commons.h"

using namespace MatrixEngine;
using namespace MatrixEngine::Core;

class KeyboardListener : public IO::DeviceListener::KeyboardEventListener {
public:
	~KeyboardListener() {
		std::cout << "Destroying keyboard listener.." << std::endl;
	}

	void OnKeyPressed(SDL_Keycode key) {
		switch (key) {
		case SDLK_ESCAPE:
			running = false;
			break;
		case SDLK_v:
			if (!Core::_pCurrentDevice->VSyncEnabled()) {
				std::cout << "Enabling V-Sync.." << std::endl;
				Core::_pCurrentDevice->EnableVSync();
			} 
			else {
				std::cout << "Disabling V-Sync.." << std::endl;
				Core::_pCurrentDevice->DisableVSync();
			}
			break;
		default:
			break;
		}
	}

};

#endif
