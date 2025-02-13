#pragma once
#include<iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <vector>

#include"Vertex.h"

class Field
{
public:
	Field(int S_W, int S_H, std::vector<Line>* vecLines);
	~Field();

	void SetRect(float posX, float posY, std::vector<Line>* vecLines);
	void DeleteRect(float mouseX, float mouseY, std::vector<Line>* vecLines);

	std::vector<Vertex> vertices;
	std::vector<Uint32> indices;
	std::vector<Vertex> verticesWLight;

private:
	void SetupWindowRect(std::vector<Line>* vecLines);
	void IntersectAllLines(std::vector<Line> vecLines);

};
