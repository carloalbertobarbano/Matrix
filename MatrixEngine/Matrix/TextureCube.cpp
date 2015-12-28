#include "TextureCube.h"

using namespace MatrixEngine::Graphics::Components::Textures;

TextureCube::TextureCube()
{
	texture_id = 0;
	for (int i = 0; i < 6; i++)
	{
		texture_file[i] = " ";
	}

	shader = NULL;
	shader_vert = shader_frag = NULL;
}

TextureCube::TextureCube(std::string *files)
{
	LoadFromFiles(files);
}

TextureCube::~TextureCube()
{
	if (texture_id)
		glDeleteTextures(1, &texture_id);

	if (shader)
	{
		shader->DetachShader(shader_vert);
		shader->DetachShader(shader_frag);

		delete shader_vert;
		delete shader_frag;
		delete shader;
	}
}

void TextureCube::LoadFromFiles(std::string *files)
{
	shader = new Shader::ShaderProgram();
	shader_vert = new Shader::ShaderSource();
	shader_frag = new Shader::ShaderSource();

	shader_vert->LoadFromFile(GL_VERTEX_SHADER, "data/shader/cubemap.vert");
	shader_frag->LoadFromFile(GL_FRAGMENT_SHADER, "data/shader/cubemap.frag");

	shader->AttachShader(shader_vert);
	shader->AttachShader(shader_frag);
	shader->Compile();

	Log::WriteOnStream("In TextureCube: Loading Cubemap..\n", Log::log_stream);

	int nOfColors[6];
	SDL_Surface *image[6];
	GLenum texture_format[6];

	for (int i = 0; i < 6; i++)
	{
		Log::WriteOnStream("In TextureCube::load_from_file: loading " + files[i] + "\n", Log::log_stream);
		texture_file[i] = files[i];
		image[i] = NULL;
		image[i] = IMG_Load(files[i].c_str());

		if (!image[i])
		{
			Log::WriteOnStream("\tCANNOT LOAD " + files[i] + " texture!\n", Log::error_stream);
			return;
		}
		else {
			nOfColors[i] = image[i]->format->BytesPerPixel;
			if (nOfColors[i] == 4)
			{
				if (image[i]->format->Rmask == 0x000000ff)
					texture_format[i] = GL_RGBA;
				else
					texture_format[i] = GL_RGBA;
			}
			else if (nOfColors[i] == 3)
			{
				if (image[i]->format->Rmask == 0x000000ff)
					texture_format[i] = GL_RGB;
				else
					texture_format[i] = GL_RGBA;
			}
			else
			{
				Log::WriteOnStream("TextureCube isn't true color. This will crash\n", Log::error_stream);
				return;
			}
		}
	}


	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifdef __ANDROID__
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R_OES, GL_CLAMP_TO_EDGE);
#else
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif
	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, nOfColors[i], image[i]->w, image[i]->h, 0, texture_format[i], GL_UNSIGNED_BYTE, image[i]->pixels);

	/*
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, nOfColors[0], image[0]->w, image[0]->h, 0, texture_format[0], GL_UNSIGNED_BYTE, image[0]->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, nOfColors[1], image[1]->w, image[1]->h, 0, texture_format[1], GL_UNSIGNED_BYTE, image[1]->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, nOfColors[2], image[2]->w, image[2]->h, 0, texture_format[2], GL_UNSIGNED_BYTE, image[2]->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, nOfColors[3], image[3]->w, image[3]->h, 0, texture_format[3], GL_UNSIGNED_BYTE, image[3]->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, nOfColors[4], image[4]->w, image[4]->h, 0, texture_format[4], GL_UNSIGNED_BYTE, image[4]->pixels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, nOfColors[5], image[5]->w, image[5]->h, 0, texture_format[5], GL_UNSIGNED_BYTE, image[5]->pixels);*/

	Log::WriteOnStream("Loaded\n", Log::log_stream);

}

void TextureCube::Bind()
{
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	mat4 glModelMatrix = mat4(1);
	glModelMatrix = glm::translate(glModelMatrix, vec3(-RenderPipeline::_pCurrentCamera->GetPosition().x,
		-RenderPipeline::_pCurrentCamera->GetPosition().y,
		-RenderPipeline::_pCurrentCamera->GetPosition().z));

	shader->sendUniformMatrix4fv(shader->getUniformLocation("projMat"), 1, GL_FALSE, RenderPipeline::glProjectionMatrix);
	shader->sendUniformMatrix4fv(shader->getUniformLocation("viewMat"), 1, GL_FALSE, RenderPipeline::glViewMatrix);
	shader->sendUniformMatrix4fv(shader->getUniformLocation("modMat"), 1, GL_FALSE, glModelMatrix);
	shader->sendUniform1i(shader->getUniformLocation("cubemap"), 0);

	shader->bind();

	RenderPipeline::_pCurrentTextureCube = this;
}

void TextureCube::Unbind()
{
	glDisable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	shader->unbind();
}

void TextureCube::BindUnit(int unit)
{
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0 + unit);
	Bind();
}

void TextureCube::UnbindUnit(int unit)
{
	glDisable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0 + unit);
	Unbind();
}

