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

	void OnKeyPressed(SDL_Scancode key) {
		keyPressed[key] = true;

		switch (key) {
		case SDL_SCANCODE_ESCAPE:
			running = false;
			break;
		case SDL_SCANCODE_V:
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

	void OnKeyUp(SDL_Scancode key) {
		keyPressed[key] = false;
	}


	static bool keyPressed[SDL_NUM_SCANCODES];
};

bool KeyboardListener::keyPressed[SDL_NUM_SCANCODES];

#endif
