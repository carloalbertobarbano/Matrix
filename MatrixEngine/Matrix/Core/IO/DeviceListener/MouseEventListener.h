#ifndef MOUSE_EVENT_LISTENER_H
#define MOUSE_EVENT_LISTENER_H

#include "../../../matrix_engine_internal.h"

using namespace MatrixEngine::Core::IO;

class DeviceListener::MouseEventListener {
public:
	virtual void OnMouseMove(glm::vec2 pos);
	virtual void OnKeyPressed(int button, int x, int y);
	virtual void OnKeyUp(int button, int x, int y);
};

#endif