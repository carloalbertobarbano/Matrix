#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "matrix_engine_internal.h"
#include "Device.h"
#include "VAO.h"

using namespace MatrixEngine;
using namespace MatrixEngine::Core;
using namespace MatrixEngine::Core::IO;
using namespace MatrixEngine::Graphics;
using namespace MatrixEngine::Graphics::Components::Textures;
using namespace glm;

class Graphics::Components::FrameBuffer
{
public:
	enum FrameBufferMode { RENDER_BUFFER, DEPTH_BUFFER };

	FrameBuffer();
	FrameBuffer(FrameBufferMode mode, vec2 resolution = _pCurrentDevice->GetResolution());
	~FrameBuffer();

	void Init(FrameBufferMode mode, vec2 resolution = _pCurrentDevice->GetResolution());

	void LoadShader(std::string vs, std::string fs);

	void Bind();
	void Unbind();

	void Render(vec4 pos = vec4(0.0, 0.0, _pCurrentDevice->GetResolution().x, _pCurrentDevice->GetResolution().y));

	uint GetTexture();

	Shader::ShaderSource *shader_vert, *shader_frag;
	Shader::ShaderProgram *shader;

protected:
	uint fbo;
	uint fbo_depth;
	uint fbo_texture;

	void init_depth_buffer();
	void init_texture();

	vec2 res;

	FrameBufferMode fbo_type;

	static bool constructed;
	static float vertex[18];
	static float tex_coords[12];
	static Memory::VAO vao;
};

#endif
