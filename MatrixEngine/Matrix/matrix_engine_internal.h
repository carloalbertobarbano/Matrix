#ifndef MATRIX_ENGINE_INTERNAL_H
#define MATRIX_ENGINE_INTERNAL_H

#if defined(__ANDROID__) || defined (TARGET_OS_IPHONE)
#define MOBILE
#endif

#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include <memory>
#include <thread>


#define GLM_FORCE_INLINE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include <SDL2/SDL.h>

#ifdef __ANDROID__
#include <SDL2_image/SDL_image.h>
#else

#ifndef __linux__
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#endif

#ifdef __linux__
#ifndef MOBILE
#include <GL/glew.h>
#endif
#endif

#ifdef _WIN32
#include <glew/glew.h>
#endif

#ifdef __APPLE__
#include <glew/glew.h>
#endif

#define NO_SDL_GLEXT
#ifdef MOBILE
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif



#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace MatrixEngine {
	namespace Core {
		class Device;
		extern Device *_pCurrentDevice;

		namespace IO {
			namespace DeviceListener {
				class KeyboardEventListener;
				class MouseEventListener;
				class TouchEventListener;
				class GamepadEventListener;
			}

			namespace Log {
				extern std::ofstream error_stream;
				extern std::ofstream log_stream;

				extern void WriteOnStream(std::string msg, std::ofstream &os);
				extern char *TextFileRead(std::string file);
			}
		}
	}

	namespace Graphics {
		class RenderPipeline;

		namespace Components {
			class FrameBuffer;

			namespace Textures {
				class Texture;
				class TextureCube;
			}

			namespace Materials {
				class Material;
			}

			namespace Lights {
				class Light;
			}
		}

		namespace Memory {
			class VAO;
		}


		namespace Shader {
			class ShaderSource;
			class ShaderProgram;
		}

		namespace UI {
			class Container;
			class Button;
			class Slider;
		}
	}

	namespace Scene {
		class SceneGraph;
		class SceneEntity;		
		class SceneInstance;

		extern SceneGraph *_pCurrentSceneGraph;

		namespace Components {
			class Camera;

			class MeshAsset;
			class MeshRenderer;

			class CubeMap;
		}
	}
}





#endif
