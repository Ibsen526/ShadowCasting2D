#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(void* data, int numVertices)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_STATIC_DRAW); //Allocates new memory
	//glBufferSubData(GL_ARRAY_BUFFER, numVertices, numVertices * sizeof(Vertex), data); //Changes already allocated memory

	glEnableVertexAttribArray(0); //vertex
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, x));

	glEnableVertexAttribArray(1); //color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, r));

	glBindVertexArray(0);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &bufferID);
}

void VertexBuffer::Bind()
{
	glBindVertexArray(vao);
}

void VertexBuffer::Unbind()
{
	glBindVertexArray(0);
}

void VertexBuffer::ResizeBuffer(void* data, int numVertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::DeleteBuffer()
{
	glDeleteBuffers(1, &bufferID);
}
