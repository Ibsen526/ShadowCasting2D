#include "Shader.h"

Shader::Shader(const char* vertexFileName, const char* fragmentFileName)
{
	shaderID = CreateShader(vertexFileName, fragmentFileName);
}

Shader::~Shader()
{
	glDeleteProgram(shaderID);
}

void Shader::Bind()
{
	glUseProgram(shaderID);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

GLuint Shader::GetShaderID()
{
	return shaderID;
}

GLuint Shader::CreateShader(const char* vertexFileName, const char* fragmentFileName)
{
	std::string vertexShaderCode = GetShaderCode(vertexFileName);
	std::string fragmentShaderCode = GetShaderCode(fragmentFileName);

	GLuint program = glCreateProgram();
	GLuint vertexShader = CompileShader(vertexShaderCode, GL_VERTEX_SHADER);
	GLuint fragmentShader = CompileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	return program;
}

std::string Shader::GetShaderCode(const char* path)
{
	std::string shaderCode = "";
	std::ifstream shaderFile;

	shaderFile.open(path);
	while (!shaderFile.eof())
	{
		std::string temp = "";
		getline(shaderFile, temp);
		shaderCode += "\n" + temp;
	}

	//std::cout << shaderCode << std::endl;

	shaderFile.close();

	return shaderCode;
}

GLuint Shader::CompileShader(std::string shaderCode, GLenum type)
{
	GLuint id = glCreateShader(type);

	const char* src = shaderCode.c_str();

	glShaderSource(id, 1, &src, 0);
	glCompileShader(id);

	//Compile Errors abfangen
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE)
	{
		int length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Shader compilation error: " << message << std::endl;
		delete[] message;
		return 0;
	}

	return id;
}
