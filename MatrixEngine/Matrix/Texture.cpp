#include "Texture.h"

using namespace MatrixEngine::Graphics::Components::Textures;

Texture::Texture()
{
	texture_id = 0;
	texture_file = "none";
	glTexture = false;
	texture_unit = 0;

}

Texture::Texture(std::string file)
{
	LoadFromFile(file);
}

Texture::~Texture()
{
    if(texture_id)
	{
	    std::cout << "Destroying texture.."<< std::endl;
		glDeleteTextures(1,&texture_id);
	}
}

void Texture::LoadFromFile(std::string file)
{
	Log::WriteOnStream("Loading texture "+file+"\n", Log::log_stream);

	SDL_Surface *texture = NULL;
	int channels = 0;
	GLenum format;

	texture = IMG_Load(file.c_str());

	if(!texture)
	{
		Log::WriteOnStream("Could not load texture: " + file + "\n", Log::error_stream);
		return;
	}

	texture_file = file;
	channels = texture->format->BytesPerPixel;

	if(channels == 4)
	{
		if(texture->format->Rmask == 0x000000ff)
		{
			format = GL_RGBA;
		}
		else
		{
			format = GL_BGRA;
		}
	}
	else if(channels == 3)
	{
        if(texture->format->Rmask == 0x000000ff)
		{
			format = GL_RGB;
		}
		else
		{
			format = GL_BGR;
		}
	}
	else
	{
		Log::WriteOnStream("Texture " + file + " isn't true color. This will crash\n", Log::error_stream);
	}

	SDL_Log("Texture width: %d\n", texture->w);
	SDL_Log("Texture height: %d\n", texture->h);

	glGenTextures(1,&texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, (channels == 4 ? GL_RGBA : GL_RGB), texture->w, texture->h, 0, format, GL_UNSIGNED_BYTE, texture->pixels);

	SDL_FreeSurface(texture);

	Log::WriteOnStream("Loaded\n", Log::log_stream);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D,texture_id);
}

void Texture::BindUnit(int unit)
{
	glActiveTexture(GL_TEXTURE0+unit);
	Bind();
	texture_unit = unit;
	glTexture = true;
}

void Texture::Unbind()
{
	if(glTexture)
	{
		glActiveTexture(GL_TEXTURE0+texture_unit);
		glTexture = false;
	}

	glBindTexture(GL_TEXTURE_2D,0);
}
