#include "MouseListener.h"

bool MouseListener::updateCamera = false;

MouseListener::MouseListener()
{
	sensibility = 0.1;
}

void MouseListener::OnMouseMove(glm::vec2 currPos)
{
	this->prevPos = this->pos;
	this->pos = currPos;


	if (RenderPipeline::_pCurrentCamera && this->updateCamera)
	{
		glm::vec2 middle = _pCurrentDevice->GetResolution() / vec2(2.0);
		float yaw   = RenderPipeline::_pCurrentCamera->GetYaw() + sensibility * (pos.x - middle.x);
		float pitch = RenderPipeline::_pCurrentCamera->GetPitch() + sensibility * (pos.y - middle.y);

		RenderPipeline::_pCurrentCamera->SetYaw(yaw);
		RenderPipeline::_pCurrentCamera->SetPitch(pitch);

		SDL_WarpMouseInWindow(Core::_pCurrentDevice->GetWindow(), middle.x, middle.y);
	}
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
