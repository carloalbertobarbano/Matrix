#ifndef GL_DEVICE_H
#define GL_DEVICE_H

#include "matrix_engine_internal.h"

using namespace MatrixEngine::Core::IO;

class MatrixEngine::Core::Device {
	public :
		Device();
		Device(Uint32 flags, int glMajorVersion, int glMinorVersion, glm::vec2 resolution, bool fullscreen = false);
		~Device();

		bool Construct(Uint32 flags, int glMajorVersion, int glMinorVersion, glm::vec2 resolution, bool fullscreen = false);
	
		bool SetResolution(glm::vec2 resolution, bool fullscreen = false);
		glm::vec2 inline GetResolution() { return resolution; }

		inline SDL_Window *GetWindow() { return window;  }
		inline SDL_GLContext GetGLContext() { return glContext; }

		bool SetGLVersion(int majorVersion, int minorVersion);
        int GetHighestAvailableGLMajorVersion();
        int GetHighestAvailableGLMinorVersion();
		inline int GetCurrentGLMajorVersion() { return glMajorVersion; }
		inline int GetCurrentGLMinorVersion() { return glMinorVersion; }
		
		void SetCaption(std::string caption, std::string icon = "");

		void EnableVSync();
		void DisableVSync();
		bool VSyncEnabled() { return vsync; }

		void RegisterKeyboardListener(std::shared_ptr<DeviceListener::KeyboardEventListener> listener);
		void RegisterMouseListener(std::shared_ptr<DeviceListener::MouseEventListener> listener);
		void RegisterTouchListener(std::shared_ptr<DeviceListener::TouchEventListener> listener);
		void RegisterGamepadListener(std::shared_ptr<DeviceListener::GamepadEventListener> listener);

        void HandleInput();
	private :
		void InitializeGL();
		void Cleanup();

	protected :
		glm::vec2 resolution;

		SDL_Window *window;
		SDL_GLContext glContext;
		int glMajorVersion;
		int glMinorVersion;

		bool vsync;

        std::shared_ptr<IO::DeviceListener::KeyboardEventListener> keyboardListener;
		std::shared_ptr<IO::DeviceListener::MouseEventListener>	   mouseListener;
		std::shared_ptr<IO::DeviceListener::TouchEventListener>    touchListener;
		std::shared_ptr<IO::DeviceListener::GamepadEventListener>  gamepadListener;
};

#endif
