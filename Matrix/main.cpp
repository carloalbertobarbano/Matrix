#include "../MatrixEngine/Matrix/Matrix.h"

#include "../MatrixEngine/Commons.h"
#include "../MatrixEngine/unit/KeyboardListener.h"
#include "../MatrixEngine/unit/MouseListener.h"
#include "../MatrixEngine/unit/ShaderLoad.h"
#include "../MatrixEngine/unit/FrameBufferTest.h"
#include "../MatrixEngine/unit/AssimpLoad.h"
#include "../MatrixEngine/unit/SceneGraphTest.h"
#include "../MatrixEngine/unit/CameraTest.h"

using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Graphics;

int main(int argc, char **argv) {
    std::cout << argv[0] << std::endl;

	int glMajorVersion = 3;
	int glMinorVersion = 3;

    std::shared_ptr<MatrixEngine::Core::Device> GLDevice(new Core::Device(SDL_INIT_EVERYTHING, glMajorVersion, glMinorVersion, glm::vec2(1600, 900), false));
	GLDevice->EnableVSync();

	std::shared_ptr<KeyboardListener> keyboardListener(new KeyboardListener());
	Core::_pCurrentDevice->RegisterKeyboardListener(keyboardListener);

	std::shared_ptr<MouseListener> mouseListener(new MouseListener());
	Core::_pCurrentDevice->RegisterMouseListener(mouseListener);

	RenderPipeline::Construct();

    RunUnitFrameBufferTest();
	RunUnitAssimpLoad();
    RunUnitSceneGraph();
	RunUnitCameraTest();

	return 0;
}
