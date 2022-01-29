#pragma once
#include<iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL.h>
#include <vector>
#include <cmath>
#include <array>

#include"Vertex.h"

class LightMap
{
public:
	void CalcShadows(float mouseX, float mouseY, std::vector<Line> vecLines, std::vector<Vertex> vertices);
	
	std::vector<Vertex> vertices;
	std::vector<Uint32> indices;

private:
	Line lightRay;
	Uint32 centerVecIndex = 0;
	float currentIntersectionX = 0, currentIntersectionY = 0; //aktueller Schnittpunkt mit der nächsten Linie

	void Intersect(float vertX, float vertY, float mouseX, float mouseY, std::vector<Line> vecLines);
	Uint32 IntersectLines(Line lightRay, std::vector<Line> vecLines);
	void SortVertices(float mouseX, float mouseY);
};
