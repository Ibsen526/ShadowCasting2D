#pragma once
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <SDL2/SDL.h>

class IndexBuffer
{
public:
	IndexBuffer() { bufferID = 0; }
	IndexBuffer(void* data, int numVertices, Uint8 indexSize);
	~IndexBuffer();

	void Bind();
	void Unbind();

	void ResizeBuffer(void* data, int numVertices, Uint8 indexSize);
	void DeleteBuffer();

private:
	GLuint bufferID;
};
