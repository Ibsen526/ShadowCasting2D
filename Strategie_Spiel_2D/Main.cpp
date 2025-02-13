#include<iostream>
#include <GL/glew.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
#include "Util.h"

// Exit window crashes!!

void OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei lenght, const GLchar* message, const void* userParam) //Bei Fehler ruft OpenGl diese Funktion auf
{
	if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM)
		std::cout << "[OpenGL Error] " << message << std::endl;
}

#define WHITE 1.0f
#define BLACK 0.0f

void DrawOneLight(Util& util, LightMap& light, float x, float y, float xn, float yn)
{
	light.vertices.clear();
	light.indices.clear();

	light.CalcShadows(xn, yn,
		util.vecLines, util.field.verticesWLight);
	glUniform1f(util.lightRadiusUniformId, light.power);
	glUniform1f(util.lightIntensityUniformId, light.intensity);

	VertexBuffer lightVertexBuffer = VertexBuffer(&light.vertices[0], (int)light.vertices.size());
	IndexBuffer lightIndexBuffer = IndexBuffer(&light.indices[0], (int)light.indices.size(), sizeof(light.indices[0]));

	glUniform2f(util.lightPosUniformId, x, (float)util.SH - y); 
	glUniform3f(util.lightColorUniformId, light.red, light.green, light.blue);

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_DST_ALPHA, GL_SRC_ALPHA, GL_DST_ALPHA);
	Util::DrawVerticesWithIndex(&lightVertexBuffer, &lightIndexBuffer, light.indices.size());
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_DST_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Util::DrawVerticesWithIndex(util.fieldVbo, util.fieldIbo, util.field.indices.size());
}

void CalcShadows(Util& util)
{
	Util::ClearScreen(BLACK);

	//Blend in the lights
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	util.lightShader->Bind();

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	DrawOneLight(util, util.playerLight, mouseX, mouseY,
		(float)mouseX / (float)util.SW * 2.0f - 1.0f, 
		-((float)mouseY / (float)util.SH * 2.0f - 1.0f));

	for (LightMap& lm : util.envLights)
	{
		DrawOneLight(util, lm, 
			(lm.posX + 1.0f) * util.SW / 2.0f,
			(lm.posY + 1.0f) * util.SH / 2.0f,
			lm.posX, -lm.posY);
	}

	util.lightShader->Unbind();


	//Blend the scene/ objects in
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
	util.creationShader->Bind();
	Util::DrawVerticesWithIndex(util.fieldVbo, util.fieldIbo, util.field.indices.size());
	util.creationShader->Unbind();

	glDisable(GL_BLEND);

	SDL_GL_SwapWindow(util.window);
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Farbkanal hat 8 bit; 0 bis 255
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); //Bitgröße des Buffers; jeder Pixel braucht 32bit; 4 Werte pro Pixel, mit je 8bit
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	Util util = Util();
	srand(time(NULL));

	if (glewInit() != GLEW_OK)
		return 0;

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLDebugCallback, 0);
#endif

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	//glBlendEquation(GL_MAX);

	util.SetupShader();
	util.creationShader->Bind();

	std::cout << "R ... switch creation and light mode" << "\n\n";

	util.InitLightSources();

	Util::ClearScreen(WHITE);
	Util::DrawVerticesWithIndex(util.fieldVbo, util.fieldIbo, util.field.indices.size());
	SDL_GL_SwapWindow(util.window);
	util.creationShader->Unbind();

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
					CalcShadows(util);
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT && !creationMode)
				{
					CalcShadows(util);
				}
				else if (event.button.button == SDL_BUTTON_LEFT && creationMode)
				{
					util.creationShader->Bind();
					int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);

					util.field.SetRect(((float)mouseX / (float)util.SW * 2.0f) - 1.0f, (((float)mouseY / (float)util.SH * 2.0f) - 1.0f) * (-1), &util.vecLines);

					util.fieldVbo->ResizeBuffer(&util.field.vertices[0], (int)util.field.vertices.size());
					util.fieldIbo->ResizeBuffer(&util.field.indices[0], (int)util.field.indices.size(), sizeof(util.field.indices[0]));


					Util::ClearScreen(WHITE);
					Util::DrawVerticesWithIndex(util.fieldVbo, util.fieldIbo, util.field.indices.size());
					SDL_GL_SwapWindow(util.window);
					util.creationShader->Unbind();
				}
				else if (event.button.button == SDL_BUTTON_RIGHT && creationMode)
				{
					util.creationShader->Bind();
					int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);

					util.field.DeleteRect(((float)mouseX / (float)util.SW * 2.0f) - 1.0f, (((float)mouseY / (float)util.SH * 2.0f) - 1.0f) * (-1), &util.vecLines);

					util.fieldVbo->ResizeBuffer(&util.field.vertices[0], (int)util.field.vertices.size());
					if(util.field.indices.size() != 0)
						util.fieldIbo->ResizeBuffer(&util.field.indices[0], (int)util.field.indices.size(), sizeof(util.field.indices[0]));


					Util::ClearScreen(WHITE);
					Util::DrawVerticesWithIndex(util.fieldVbo, util.fieldIbo, util.field.indices.size());
					SDL_GL_SwapWindow(util.window);
					util.creationShader->Unbind();
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (event.button.button == SDL_BUTTON_LEFT && !creationMode)
				{
					Util::ClearScreen(BLACK);
					SDL_GL_SwapWindow(util.window);
				}
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r)
			{
				if (creationMode)
				{
					creationMode = false;
					SDL_ShowCursor(false);

					Util::ClearScreen(BLACK);
					SDL_GL_SwapWindow(util.window);
				}
				else
				{
					util.creationShader->Bind();
					creationMode = true;
					SDL_ShowCursor(true);

					Util::ClearScreen(WHITE);
					Util::DrawVerticesWithIndex(util.fieldVbo, util.fieldIbo, util.field.indices.size());
					SDL_GL_SwapWindow(util.window);
					util.creationShader->Unbind();
				}
			}
			else if (event.type == SDL_QUIT)
			{
				close = true;
			}
		}
	}
	
	SDL_Quit();
	return 0;
}
