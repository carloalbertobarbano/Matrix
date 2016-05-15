#ifndef TEXTURE_H
#define TEXTURE_H

#include "matrix_engine_internal.h"
#include "Shader.h"
#include "RenderPipeline.h"

using namespace MatrixEngine::Core::IO;
using namespace MatrixEngine::Graphics;
using namespace MatrixEngine::Graphics::Components;

class Textures::Texture
{
  public :
	  Texture();
	  Texture(std::string file);
	  ~Texture();

	  void LoadFromFile(std::string file);

	  void Bind();
	  void BindUnit(int unit);
	  void Unbind();

	  operator GLuint() { return texture_id; }
	  GLuint GetTexture(){ return texture_id; }

      std::string GetName(){ return texture_file; }

      static void ActiveAndBindUnit(int unit,GLuint texture)
      {
          glActiveTexture(GL_TEXTURE0+unit);
          glBindTexture(GL_TEXTURE_2D,texture);
      }

  protected :
	  GLuint texture_id;
	  std::string texture_file;

	  bool glTexture;
	  int texture_unit;
};

#endif
