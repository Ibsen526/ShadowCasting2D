#include<iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include"Vertex.h"

class VertexBuffer
{
public:
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