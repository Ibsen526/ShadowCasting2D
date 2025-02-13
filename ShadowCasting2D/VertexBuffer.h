#include<iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include"Vertex.h"

class VertexBuffer
{
public:
	VertexBuffer() { bufferID = 0; vao = 0; }
	VertexBuffer(void* data, int numVertices);
	~VertexBuffer();

	void Bind();
	void Unbind();

	void ResizeBuffer(void* data, int numVertices);
	void DeleteBuffer();


private:
	Uint32 bufferID;
	Uint32 vao;

};