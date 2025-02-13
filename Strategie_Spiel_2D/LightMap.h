#pragma once
#include<iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <array>

#include"Vertex.h"

class LightMap
{
public:
	LightMap(float x, float y, float p, float i, float r, float g, float b, bool f) : 
		posX(x), posY(y), power(p), intensity(i), red(r), green(g), blue(b), flare(f) {};
	void CalcShadows(float mouseX, float mouseY, std::vector<Line> vecLines, std::vector<Vertex> vertices);
	
	std::vector<Vertex> vertices;
	std::vector<Uint32> indices;

	float posX;
	float posY;
	float power;
	float intensity;
	float red, green, blue;
	bool flare;

private:
	Line lightRay = { 0,0,0,0 };
	Uint32 centerVecIndex = 0;
	float currentIntersectionX = 0, currentIntersectionY = 0; //aktueller Schnittpunkt mit der nächsten Linie

	void Intersect(float vertX, float vertY, float mouseX, float mouseY, std::vector<Line> vecLines);
	Uint32 IntersectLines(Line lightRay, std::vector<Line> vecLines);
	void SortVertices(float mouseX, float mouseY);
};
