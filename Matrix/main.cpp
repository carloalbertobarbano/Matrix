#include "Matrix/Matrix.h"

#include "Commons.h"
#include "unit/KeyboardListener.h"
#include "unit/MouseListener.h"
#include "unit/ShaderLoad.h"
#include "unit/FrameBufferTest.h"
#include "unit/AssimpLoad.h"

using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Graphics;

int main(int argc, char **argv) {
	int glMajorVersion = 3;
	int glMinorVersion = 3;

    std::shared_ptr<MatrixEngine::Core::Device> GLDevice(new Core::Device(SDL_INIT_EVERYTHING, glMajorVersion, glMinorVersion, glm::vec2(1600, 900), false));
	GLDevice->EnableVSync();
	
	std::shared_ptr<KeyboardListener> keyboardListener(new KeyboardListener());
	Core::_pCurrentDevice->RegisterKeyboardListener(keyboardListener);

	std::shared_ptr<MouseListener> mouseListener(new MouseListener());
	Core::_pCurrentDevice->RegisterMouseListener(mouseListener);

	RenderPipeline::Construct();

	RunUnitAssimpLoad();

	return 0;
}
