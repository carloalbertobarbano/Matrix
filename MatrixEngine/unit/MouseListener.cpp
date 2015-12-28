#include "MouseListener.h"

void MouseListener::OnMouseMove(glm::vec2 currPos)
{
	this->prevPos = this->pos;
	this->pos = currPos;

	//std::cout << "Mouse moved to " << currPos.x << ", " << currPos.y << std::endl;
}

void MouseListener::OnKeyPressed(int button, int x, int y)
{
	switch (button) {
		case SDL_BUTTON_LEFT:
			this->leftButtonPressed = true;
			std::cout << "Pressed left button at " << x << ", " << y << std::endl;
			break;

		case SDL_BUTTON_RIGHT:
			this->rightButtonPressed = true;
			std::cout << "Pressed right button at " << x << ", " << y << std::endl;
			break;

		case SDL_BUTTON_MIDDLE:
			this->middleButtonPressed = true;
			std::cout << "Pressed middle button at " << x << ", " << y << std::endl;
			break;

		default: break;
	}
}

void MouseListener::OnKeyUp(int button, int x, int y)
{
	switch (button) {
	case SDL_BUTTON_LEFT:
		this->leftButtonPressed = false;
		break;

	case SDL_BUTTON_RIGHT:
		this->rightButtonPressed = false;
		break;

	case SDL_BUTTON_MIDDLE:
		this->middleButtonPressed = false;
		break;

	default: break;
	}
}
