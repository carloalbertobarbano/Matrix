#ifndef UNIT_MOUSE_LISTENER_H
#define UNIT_MOUSE_LISTENER_H

#include "../Matrix/Matrix.h"

using namespace MatrixEngine::Core::IO;

class MouseListener : public DeviceListener::MouseEventListener {
	public :
		MouseListener();

		virtual void OnMouseMove(glm::vec2 pos);
		virtual void OnKeyPressed(int button, int x, int y);
		virtual void OnKeyUp(int button, int x, int y);
	
		glm::vec2 GetPos() { return pos; }
		glm::vec2 GetPrevPos() { return prevPos; }
		
		bool LeftButtonPressed() { return leftButtonPressed; }
		bool RightButtonPressed() { return rightButtonPressed; }
		bool MiddleButtonPressed() { return middleButtonPressed; }


		float sensibility;
		static bool updateCamera;
	protected :
		glm::vec2 pos;
		glm::vec2 prevPos;

		bool leftButtonPressed;
		bool rightButtonPressed;
		bool middleButtonPressed;

};


#endif
