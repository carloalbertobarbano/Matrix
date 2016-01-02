#ifndef TEXTURE_CUBE_H
#define TEXTURE_CUBE_H

#include "matrix_engine_internal.h"
#include "Shader.h"
#include "Camera.h"
#include "RenderPipeline.h"

using namespace MatrixEngine::Core::IO;
using namespace MatrixEngine::Graphics;
using namespace MatrixEngine::Graphics::Components;


class Textures::TextureCube
{
public:
	TextureCube();
	TextureCube(std::string *files);
	~TextureCube();

	void LoadFromFiles(std::string *files);
	void Bind();
	void Unbind();

	void BindUnit(int unit);
	void UnbindUnit(int unit);

	operator GLuint() { return texture_id; }
	GLuint GetTexture() { return texture_id; }

protected:
	GLuint texture_id;
	std::string texture_file[6];
};

#endif
