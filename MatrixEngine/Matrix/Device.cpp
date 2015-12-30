#include "Device.h"

#include "KeyboardEventListener.h"
#include "MouseEventListener.h"

MatrixEngine::Core::Device::Device()
{
    this->keyboardListener = NULL;
    this->mouseListener = NULL;
    this->gamepadListener = NULL;
    this->touchListener = NULL;
}

MatrixEngine::Core::Device::Device(Uint32 flags, int glMajorVersion, int glMinorVersion, glm::vec2 resolution, bool fullscreen)
{
	this->Construct(flags, glMajorVersion, glMinorVersion, resolution, fullscreen);
}

MatrixEngine::Core::Device::~Device()
{
	Log::WriteOnStream("Destroying device..\n", Log::log_stream);

	this->Cleanup();

	Log::error_stream.close();
	Log::log_stream.close();
}

void MatrixEngine::Core::Device::Cleanup()
{
	IMG_Quit();
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


bool MatrixEngine::Core::Device::Construct(Uint32 flags, int glMajorVersion, int glMinorVersion, glm::vec2 resolution, bool fullscreen)
{
	//std::cout << "MatrixEngine::Core::Device(): Constructing OpenGL device.." << std::endl;
	SDL_Log("MatrixEngine::Core::Device(): Constructing OpenGL device..");

	Core::_pCurrentDevice = this;

    this->keyboardListener = NULL;
    this->mouseListener = NULL;
    this->gamepadListener = NULL;
    this->touchListener = NULL;
    
	Log::error_stream.open("error.log", std::ios::out);
	Log::log_stream.open("matrix.log", std::ios::out);
    if(!Log::error_stream.is_open() || !Log::log_stream.is_open())
        std::cout << "Canot open log files!" << std::endl;
   

	if (SDL_Init(flags) < 0) {
		MatrixEngine::Core::IO::Log::WriteOnStream("Device::Construct(): Failed to init SDL\n", Log::error_stream);
		return false;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
		MatrixEngine::Core::IO::Log::WriteOnStream("Device::Construct(): Failed to init SDL_image\n", Log::error_stream);
	}

	if (TTF_Init() < 0) {
		MatrixEngine::Core::IO::Log::WriteOnStream("Device::Construct(): Failed to init SDL_tff\n", Log::error_stream);
	}
   
    if(glMajorVersion == 0 && glMajorVersion == 0) {
        glMajorVersion = this->GetHighestAvailableGLMajorVersion();
        glMinorVersion = this->GetHighestAvailableGLMinorVersion();
    }
	this->SetGLVersion(glMajorVersion, glMinorVersion);

    
    window = SDL_CreateWindow("Matrix Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resolution.x, resolution.y, SDL_WINDOW_OPENGL);
    if (!SetResolution(resolution, fullscreen)) {
        Log::WriteOnStream("Device::Construct(): Cannot set resolution!\n", Log::error_stream);
    }
    this->SetResolution(resolution, fullscreen);
    
    
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL) {
		Log::WriteOnStream("Device::Construct(): Failed to create GL Context!\n", Log::error_stream);
		return false;
	}
	SDL_GL_MakeCurrent(window, glContext);

	this->InitializeGL();

    
    std::stringstream msg;
    msg << "OpenGL version: " << glGetString(GL_VERSION) << ". ";
    msg << "GLSL Version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    Log::WriteOnStream(msg.str(), Log::log_stream);
    
	return true;
}

bool MatrixEngine::Core::Device::SetResolution(glm::vec2 resolution, bool fullscreen)
{
	this->resolution = resolution;
	SDL_SetWindowSize(this->window, resolution.x, resolution.y);

	if (fullscreen) {
		if (SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0) {
			Log::WriteOnStream("Device::SetResolution(): Cannot set fullscreen mode!", Log::error_stream);
			return false;
		}
	}
	else {
		if (SDL_SetWindowFullscreen(window, 0) < 0) {
			Log::WriteOnStream("Device::SetResolution(): Cannot set windowed mode!", Log::error_stream);
			return false;
		}
	}

	return true;
}

bool MatrixEngine::Core::Device::SetGLVersion(int majorVersion, int minorVersion)
{
	/* TODO: Manage ES context */
#ifdef MOBILE
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion) < 0) {
		Log::WriteOnStream("Device::SetGLVersion(): Cannot set GL major version to " + majorVersion, Log::error_stream);
		return false;
	}

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersion) < 0) {
		Log::WriteOnStream("Device::SetGLVersion(): Cannot set GL minor version to " + minorVersion, Log::error_stream);
		return false;
	}

	return true;
}

int MatrixEngine::Core::Device::GetHighestAvailableGLMajorVersion()
{
    int highestMajor = 0;
	//glGetIntegerv(GL_MAJOR_VERSION, &highestMajor);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &highestMajor);
    return highestMajor;
}

int MatrixEngine::Core::Device::GetHighestAvailableGLMinorVersion()
{
    int highestMinor = 0;
	//glGetIntegerv(GL_MINOR_VERSION, &highestMinor);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &highestMinor);
    return highestMinor;
}



void MatrixEngine::Core::Device::SetCaption(std::string caption, std::string icon)
{
	SDL_SetWindowTitle(window, caption.c_str());
	/* TODO: icon*/
}

void MatrixEngine::Core::Device::EnableVSync()
{
	SDL_GL_SetSwapInterval(1);
	vsync = true;
}

void MatrixEngine::Core::Device::DisableVSync()
{
	SDL_GL_SetSwapInterval(0);
	vsync = false;
}

void MatrixEngine::Core::Device::RegisterKeyboardListener(std::shared_ptr<DeviceListener::KeyboardEventListener> listener)
{
	this->keyboardListener = listener;
}

void MatrixEngine::Core::Device::RegisterMouseListener(std::shared_ptr<DeviceListener::MouseEventListener> listener)
{
	this->mouseListener = listener;
}

void MatrixEngine::Core::Device::RegisterTouchListener(std::shared_ptr<DeviceListener::TouchEventListener> listener)
{
	this->touchListener = listener;
}

void MatrixEngine::Core::Device::RegisterGamepadListener(std::shared_ptr<DeviceListener::GamepadEventListener> listener)
{
	this->gamepadListener = listener;
}

void MatrixEngine::Core::Device::InitializeGL()
{
#if !defined(__APPLE__) && !defined(__ANDROID__)
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		Log::WriteOnStream("Cannot initialize glew: " + std::string((const char*)glewGetErrorString(res)), Log::error_stream);
	}
#endif
}

void MatrixEngine::Core::Device::HandleInput()
{
    if(this->keyboardListener == NULL)
        keyboardListener = std::make_shared<IO::DeviceListener::KeyboardEventListener>();
    if(this->mouseListener == NULL)
        mouseListener = std::make_shared<IO::DeviceListener::MouseEventListener>();
    //if(this->touchListener == NULL)
    //    touchListener = std::make_shared<IO::DeviceListener::TouchEventListener>();
    //if(this->gamepadListener == NULL)
    //    gamepadListener = std::make_shared<IO::DeviceListener::GamepadEventListener>();
    
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				// ?
				break;

			case SDL_KEYDOWN:
				this->keyboardListener->OnKeyPressed(event.key.keysym.scancode);
				break;

			case SDL_KEYUP:
				this->keyboardListener->OnKeyUp(event.key.keysym.scancode);
				break;

			case SDL_MOUSEMOTION:
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				glm::vec2 pos(x, y);
				this->mouseListener->OnMouseMove(pos);
			}
			break;

			case SDL_MOUSEBUTTONDOWN:
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				this->mouseListener->OnKeyPressed(event.button.button, x, y);
			}
			break;

			case SDL_MOUSEBUTTONUP:
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				this->mouseListener->OnKeyUp(event.button.button, x, y);
			}
			break;
	
			default: break;
		}
	}

}
