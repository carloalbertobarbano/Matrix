#include "VAO.h"

using namespace MatrixEngine::Graphics::Memory;

VAO::VAO()
{
	vao = 0;
    buffer = NULL;
    constructed = false;
	vert_count = 0;

	SDL_Log("VAO::VAO() PLEASE FIX MY BUG (rendering with indices)\n");
}

VAO::~VAO()
{
   if(constructed)
#ifdef __APPLE__
   glDeleteVertexArraysAPPLE(1,&vao);
#else
#ifndef __ANDROID__
   glDeleteVertexArrays(1,&vao);
#endif
#endif
    
   if(buffer)
    delete []buffer;
}

void VAO::Construct(int buff_n)
{
	buffer = new GLuint[buff_n];
#ifdef __APPLE__
    glGenVertexArraysAPPLE(1,&vao);
#else
#ifndef __ANDROID__
    glGenVertexArrays(1,&vao);
#endif
#endif
	
	Bind();

	glGenBuffers(buff_n,buffer);

	constructed = true;
}

void VAO::DeleteBuffer(int buff_n)
{
	glDeleteBuffers(1,&buffer[buff_n]);
}

void VAO::GenBuffer(int buff_n)
{
   glGenBuffers(1,&buffer[buff_n]);
}

void VAO::ResetBuffer(int buff_n)
{
	DeleteBuffer(buff_n);
    GenBuffer(buff_n);
}

void VAO::Bind()
{
#ifdef __APPLE__
    glBindVertexArrayAPPLE(vao);
#else
#ifndef __ANDROID__
	glBindVertexArray(vao);
#endif
#endif
}

void VAO::Unbind()
{
#ifdef __APPLE__
    glBindVertexArrayAPPLE(0);
#else
#ifndef __ANDROID__
	glBindVertexArray(0);
#endif
#endif
}

void VAO::BufferData(int buff,GLenum target,GLsizeiptr size,GLvoid *data,GLenum mode)
{
	glBindBuffer(target,buffer[buff]);
	glBufferData(target,size,data,mode);
}

void VAO::VertexAttribute(int location,int size,GLenum type,GLboolean normalized,GLsizei stride,GLvoid *pointer)
{
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location,size,type,normalized,stride,pointer);
}



void VAO::DrawArrays(GLenum mode,GLint first,GLsizei count,void *indices)
{
	Bind();
	if(!indices)
	  glDrawArrays(mode,first,count);
	else
	  glDrawElements(mode, count, GL_FLOAT, indices);
    Unbind();
}
