#pragma once
#include<iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/GL.h>
#include <GL/GLU.h>

class Util
{
public:
	Util()
	{
		Uint32 winFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
		window = SDL_CreateWindow("Shadow Casting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SW, SH, winFlags);
		glContext = SDL_GL_CreateContext(window);
	}

	~Util()
	{
		delete lightShader;
		delete creationShader;
		delete fieldVbo;
		delete fieldIbo;
		SDL_DestroyWindow(window);
	}

	void SetupShader()
	{
		creationShader = new Shader("shader.vert", "creationShader.frag");
		lightShader = new Shader("shader.vert", "lightShader.frag");

		lightPosUniformId =	glGetUniformLocation(lightShader->GetShaderID(), "u_lightPos");
		lightRadiusUniformId = glGetUniformLocation(lightShader->GetShaderID(), "u_lightRadius");
		lightIntensityUniformId = glGetUniformLocation(lightShader->GetShaderID(), "u_lightIntensity");
		lightColorUniformId = glGetUniformLocation(lightShader->GetShaderID(), "u_lightColor");

		fieldVbo = new VertexBuffer(&field.vertices[0], (int)field.vertices.size());
		fieldIbo = new IndexBuffer(&field.indices[0], (int)field.indices.size(), sizeof(field.indices[0]));
	}

	void InitLightSources()
	{
		envLights.push_back(LightMap(0.1f, 0.1f, 150.0f, 0.2f, 0.7f, 1.0f, 0.0f, false));
		envLights.push_back(LightMap(0.2f, -0.6f, 100.0f, 0.2f, 0.7f, 1.0f, 0.0f, true));
	}

	static void ClearScreen(float white)
	{
		glClearColor(white, white, white, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	static void DrawVerticesWithIndex(VertexBuffer* v, IndexBuffer* i, size_t iSize)
	{
		v->Bind();
		i->Bind();
		glDrawElements(GL_TRIANGLES, iSize, GL_UNSIGNED_INT, 0);
		i->Unbind();
		v->Unbind();
	}

	SDL_Window* window = nullptr;
	SDL_GLContext glContext = NULL;

	int lightPosUniformId = 0;
	int lightRadiusUniformId = 0;
	int lightIntensityUniformId = 0;
	int lightColorUniformId = 0;

	const Uint16 SW = 1280;
	const Uint16 SH = 720;

	std::vector<Line> vecLines;

	Field field = Field(SW, SH, &vecLines);
	LightMap playerLight = LightMap(0.0f, 0.0f, 300.0f, 1.0f, 0.9f, 0.8f, 0.0f, false);
	std::vector<LightMap> envLights;

	VertexBuffer* fieldVbo = nullptr;
	IndexBuffer* fieldIbo = nullptr;
	Shader* creationShader = nullptr;
	Shader* lightShader = nullptr;

};
