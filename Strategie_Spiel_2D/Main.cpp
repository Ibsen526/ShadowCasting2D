#include<iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <cmath>
#include <vector>

#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Field.h"
#include "LightMap.h"

void OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei lenght, const GLchar* message, const void* userParam) //Bei Fehler ruft OpenGl diese Funktion auf
{
	std::cout << "[OpenGL Error] " << message << std::endl;
}

#define WHITE true
#define BLACK false

void glClearScreen(bool backIsWhite)
{
	if (backIsWhite)
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	else
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void glDrawVerticesWithIndex(SDL_Window* window, VertexBuffer* v, IndexBuffer* i, std::vector<Uint32> indices)
{
	v->Bind();
	i->Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	i->Unbind();
	v->Unbind();
}

int main()
{
	const Uint16 SCREEN_W = 1280;
	const Uint16 SCREEN_H = 720;
	std::vector<Line> vecLines;


	SDL_Window* window = nullptr;
	SDL_GLContext glContext = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Farbkanal hat 8 bit; 0 bis 255
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); //Bitgröße des Buffers; jeder Pixel braucht 32bit; 4 Werte pro Pixel, mit je 8bit
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


	Uint32 winFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	window = SDL_CreateWindow("Shadow Casting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, winFlags);
	glContext = SDL_GL_CreateContext(window);

	if (glewInit() != GLEW_OK)
		return 0;

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLDebugCallback, 0);
#endif


	Field field = Field(SCREEN_W, SCREEN_H, &vecLines);
	LightMap light = LightMap();

	VertexBuffer fieldVertexBuffer = VertexBuffer(&field.vertices[0], (int)field.vertices.size());
	IndexBuffer fieldIndexBuffer = IndexBuffer(&field.indices[0], (int)field.indices.size(), sizeof(field.indices[0]));
	Shader shader = Shader("shader.vert", "shader.frag");
	shader.Bind();


	std::cout << "R ... switch creation and light mode" << std::endl << std::endl;

	//Get the mouse middle uniform
	int mousePosUniformId;
	mousePosUniformId = glGetUniformLocation(shader.GetShaderID(), "a_mouseMiddle");
	glUniform2f(mousePosUniformId, (float)(SCREEN_W / 2), (float)(SCREEN_H / 2)); //Sets the mouse uniform


	glClearScreen(WHITE);
	glDrawVerticesWithIndex(window, &fieldVertexBuffer, &fieldIndexBuffer, field.indices);
	SDL_GL_SwapWindow(window);

	bool creationMode = true;
	bool close = false; 
	while(!close)
	{

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_MOUSEMOTION)
			{
				if (event.button.button == SDL_BUTTON_LEFT && !creationMode)
				{
					light.vertices.clear();
					light.indices.clear();

					int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);

					light.CalcShadows(((float)mouseX / (float)SCREEN_W * 2.0f) - 1.0f, (((float)mouseY / (float)SCREEN_H * 2.0f) - 1.0f) * (-1), vecLines, field.verticesWLight);

					VertexBuffer lightVertexBuffer = VertexBuffer(&light.vertices[0], (int)light.vertices.size());
					IndexBuffer lightIndexBuffer = IndexBuffer(&light.indices[0], (int)light.indices.size(), sizeof(light.indices[0]));
										
					glUniform2f(mousePosUniformId, (float)mouseX, (float)(SCREEN_H - mouseY)); //Sets the mouse uniform

					glClearScreen(BLACK);
					glDrawVerticesWithIndex(window, &lightVertexBuffer, &lightIndexBuffer, light.indices);
					glDrawVerticesWithIndex(window, &fieldVertexBuffer, &fieldIndexBuffer, field.indices);
					SDL_GL_SwapWindow(window);
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT && !creationMode)
				{
					int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);

					light.CalcShadows(((float)mouseX / (float)SCREEN_W * 2.0f) - 1.0f, (((float)mouseY / (float)SCREEN_H * 2.0f) - 1.0f) * (-1), vecLines, field.verticesWLight);

					VertexBuffer lightVertexBuffer = VertexBuffer(&light.vertices[0], (int)light.vertices.size());
					IndexBuffer lightIndexBuffer = IndexBuffer(&light.indices[0], (int)light.indices.size(), sizeof(light.indices[0]));

					glUniform2f(mousePosUniformId, (float)mouseX, (float)(SCREEN_H - mouseY)); //Sets the mouse uniform

					glClearScreen(BLACK);
					glDrawVerticesWithIndex(window, &lightVertexBuffer, &lightIndexBuffer, light.indices);
					glDrawVerticesWithIndex(window, &fieldVertexBuffer, &fieldIndexBuffer, field.indices);
					SDL_GL_SwapWindow(window);
				}
				else if (event.button.button == SDL_BUTTON_LEFT && creationMode)
				{
					int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);

					field.SetRect(((float)mouseX / (float)SCREEN_W * 2.0f) - 1.0f, (((float)mouseY / (float)SCREEN_H * 2.0f) - 1.0f) * (-1), &vecLines);

					fieldVertexBuffer.ResizeBuffer(&field.vertices[0], (int)field.vertices.size());
					fieldIndexBuffer.ResizeBuffer(&field.indices[0], (int)field.indices.size(), sizeof(field.indices[0]));


					glClearScreen(WHITE);
					glDrawVerticesWithIndex(window, &fieldVertexBuffer, &fieldIndexBuffer, field.indices);
					SDL_GL_SwapWindow(window);
				}
				else if (event.button.button == SDL_BUTTON_RIGHT && creationMode)
				{
					int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);

					field.DeleteRect(((float)mouseX / (float)SCREEN_W * 2.0f) - 1.0f, (((float)mouseY / (float)SCREEN_H * 2.0f) - 1.0f) * (-1), &vecLines);

					fieldVertexBuffer.ResizeBuffer(&field.vertices[0], (int)field.vertices.size());
					if(field.indices.size() != 0)
						fieldIndexBuffer.ResizeBuffer(&field.indices[0], (int)field.indices.size(), sizeof(field.indices[0]));


					glClearScreen(WHITE);
					glDrawVerticesWithIndex(window, &fieldVertexBuffer, &fieldIndexBuffer, field.indices);
					SDL_GL_SwapWindow(window);
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (event.button.button == SDL_BUTTON_LEFT && !creationMode)
				{
					light.vertices.clear();
					light.indices.clear();

					glClearScreen(BLACK);
					SDL_GL_SwapWindow(window);
				}
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r)
			{
				if (creationMode)
				{
					creationMode = false;

					SDL_ShowCursor(false);

					glClearScreen(BLACK);
					SDL_GL_SwapWindow(window);
				}
				else
				{
					creationMode = true;

					SDL_ShowCursor(true);

					glClearScreen(WHITE);
					glDrawVerticesWithIndex(window, &fieldVertexBuffer, &fieldIndexBuffer, field.indices);
					SDL_GL_SwapWindow(window);
				}
			}
			else if (event.type == SDL_QUIT)
			{
				close = true;
			}
		}
	}
	
	return 0;
}
