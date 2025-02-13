#include<iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <string>
#include <fstream>


class Shader
{
public:
	Shader(const char* vertexFileName, const char* fragmentFileName);
	~Shader();

	void Bind();
	void Unbind();

	GLuint GetShaderID();

private:
	GLuint CreateShader(const char* vertexFileName, const char* fragmentFileName);
	std::string GetShaderCode(const char* path);
	GLuint CompileShader(std::string shaderCode, GLenum type);

	GLuint shaderID;
};
