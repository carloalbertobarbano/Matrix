#include "FrameBuffer.h"
#include "RenderPipeline.h"
#include "Texture.h"

using namespace MatrixEngine::Core::IO;
using namespace MatrixEngine::Graphics::Memory;
using namespace MatrixEngine::Graphics::Components;

bool   FrameBuffer::constructed = false;
float  FrameBuffer::vertex[] = { -1.0,-1.0,0.0, -1.0,1.0,0.0, 1.0,1.0,0.0, 1.0, 1.0, 0.0, 1.0,-1.0,0.0, -1.0, -1.0, 0.0 };

float  FrameBuffer::tex_coords[] = { 0.0,0.0,     0.0,1.0,     1.0,1.0,    1.0, 1.0,  1.0,0.0, 0.0, 0.0 };
VAO    FrameBuffer::vao;

FrameBuffer::FrameBuffer()
{
	shader_vert = shader_frag = NULL;
	shader = NULL;
}

FrameBuffer::FrameBuffer(FrameBuffer::FrameBufferMode mode, vec2 resolution)
{
	shader_vert = shader_frag = NULL;
	shader = NULL;
	Init(mode, resolution);
}

FrameBuffer::~FrameBuffer()
{
	if (shader)
	{
		if (shader_vert)
		{
			shader->DetachShader(shader_vert);
			delete shader_vert;
		}

		if (shader_frag)
		{
			shader->DetachShader(shader_frag);
			delete shader_frag;
		}

		delete shader;
	}

	glDeleteRenderbuffers(1, &fbo_depth);
	glDeleteTextures(1, &fbo_texture);
	glDeleteFramebuffers(1, &fbo);
}

void FrameBuffer::init_depth_buffer()
{

}

void FrameBuffer::init_texture()
{

}

void FrameBuffer::Init(FrameBuffer::FrameBufferMode mode, vec2 resolution)
{

	if (!constructed)
	{
		int vertex_location = 0;
		int texture_location = 1;

		vao.Construct(2);

		vao.BufferData(0, GL_ARRAY_BUFFER, sizeof(vertex), &vertex[0], GL_STATIC_DRAW);
		vao.VertexAttribute(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

		vao.BufferData(1, GL_ARRAY_BUFFER, sizeof(tex_coords), &tex_coords[0], GL_STATIC_DRAW);
		vao.VertexAttribute(texture_location, 2, GL_FLOAT, GL_FALSE, 0, 0);

		vao.Unbind();
		constructed = true;
	}

	Log::WriteOnStream("Constructing FBO..\n", Log::log_stream);

	fbo_type = mode;
	res = resolution;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	/** RGB texture */
	glGenTextures(1, &fbo_texture);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, res.x, res.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	/** Depth buffer*/

	glGenRenderbuffers(1, &fbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, res.x, res.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);
    

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbo_texture, 0);

	//GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};

	//glDrawBA
	//glDrawBuffers(1,drawBuffers);



	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		Log::WriteOnStream("Couldn't create frame buffer\n", Log::error_stream);
	}
	else
	{
		Log::WriteOnStream("Frame buffer correctly created\n", Log::log_stream);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::LoadShader(std::string vs, std::string fs)
{
	shader_vert = new Shader::ShaderSource();
	shader_frag = new Shader::ShaderSource();
	shader = new Shader::ShaderProgram();

	shader_vert->LoadFromFile(GL_VERTEX_SHADER, vs);
	shader_frag->LoadFromFile(GL_FRAGMENT_SHADER, fs);

	shader->AttachShader(shader_vert);
	shader->AttachShader(shader_frag);
	shader->Compile();
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	RenderPipeline::SetViewport(vec4(0.0, 0.0, res.x, res.y));
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RenderPipeline::SetViewport();
}

void FrameBuffer::Render(vec4 pos)
{
	Texture::ActiveAndBindUnit(0, fbo_texture);
	RenderPipeline::SetViewport(pos);

	shader->sendUniform1i(shader->getUniformLocation("fbo_texture"), 0);
	shader->bind();

	int fMode = RenderPipeline::FillMode;
#ifndef __ANDROID__
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
	/** RENDER */
	vao.DrawArrays(GL_TRIANGLES, 0, 6);

	
	shader->unbind();

	Texture::ActiveAndBindUnit(0, 0);

#ifndef __ANDROID__
	glPolygonMode(GL_FRONT_AND_BACK, fMode);
#endif
}

uint FrameBuffer::GetTexture()
{
	return fbo_texture;
}
