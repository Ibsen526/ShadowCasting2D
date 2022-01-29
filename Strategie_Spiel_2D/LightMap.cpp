#include"LightMap.h"

void LightMap::CalcShadows(float mouseX, float mouseY, std::vector<Line> vecLines, std::vector<Vertex> verticesField)
{
	for (Uint32 i = 0; i < verticesField.size(); i++) //Calculates the intersection vertices of all points
	{
		Intersect(verticesField[i].x, verticesField[i].y, mouseX, mouseY, vecLines);
	}

	SortVertices(mouseX, mouseY); //Sorts the vertices from smallest to the biggest angle

	//Add the center point
	vertices.push_back({ mouseX, mouseY,
		1.0f, 1.0f, 1.0f, 1.0f });
	centerVecIndex = vertices.size() - 1;

	//Sets all the indexes for the triangles 
	for (Uint32 i = 0; i < vertices.size() - 1; i++)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(centerVecIndex);
	}

	indices.push_back(vertices.size() - 2);
	indices.push_back(0);
	indices.push_back(centerVecIndex);
}

void LightMap::Intersect(float vertX, float vertY, float mouseX, float mouseY, std::vector<Line> vecLines)
{

	//Calculates the angle for the middle ray
	float x = vertX - mouseX, y = vertY - mouseY;
	float baseAngle = atan2f(y, x); 


	//Shoots 3 rays, one slightly more left and one more right than the one directly to the point

	Uint32 currentLineIndex = 0;
	for (Uint8 i = 0; i < 3; i++)
	{
		float angle = baseAngle;

		if (i == 1)
			angle += 0.001f;
		else if (i == 2)
			angle -= 0.001f;


		if(i == 0)
			lightRay = { mouseX, mouseY, vertX, vertY };
		else
			lightRay = { mouseX, mouseY, (cosf(angle) * 3.0f) + mouseX, (sinf(angle) * 3.0f) + mouseY };


		if (i == 0)
		{
			currentLineIndex = IntersectLines(lightRay, vecLines);
			vertices.push_back({ currentIntersectionX, currentIntersectionY,
				1.0f,1.0f,1.0f,1.0f });
		}
		else
		{
			Uint32 tempLineIndex = IntersectLines(lightRay, vecLines);

			if (tempLineIndex != currentLineIndex)
			{
				//Add vertex if the line of the displaced rays are intersecting a different line as the original ray 
				vertices.push_back({ currentIntersectionX, currentIntersectionY,
					1.0f,1.0f,1.0f,1.0f });
			}
		}
	}
}

Uint32 LightMap::IntersectLines(Line lightRay, std::vector<Line> vecLines)
{
	float lightRayDistanceX = lightRay.x2 - lightRay.x1;
	float lightRayDistanceY = lightRay.y2 - lightRay.y1;

	Uint32 tempLineIndex = 0;
	float min_t1 = INFINITY;

	for (Uint32 i = 0; i < vecLines.size(); i++)
	{
		float edgeDistanceX = vecLines[i].x2 - vecLines[i].x1;
		float edgeDistanceY = vecLines[i].y2 - vecLines[i].y1;


		if (fabs(edgeDistanceX - lightRayDistanceX) > 0.0f && fabs(edgeDistanceY - lightRayDistanceY) > 0.0f)
		{
			// t2 is normalised distance from line segment start to line segment end of intersect point
			float t2 = (lightRayDistanceX * (vecLines[i].y1 - lightRay.y1) + (lightRayDistanceY * (lightRay.x1 - vecLines[i].x1))) / (edgeDistanceX * lightRayDistanceY - edgeDistanceY * lightRayDistanceX);
			// t1 is normalised distance from source along ray to ray length of intersect point
			float t1 = (vecLines[i].x1 + edgeDistanceX * t2 - lightRay.x1) / lightRayDistanceX;

			// If intersect point exists along ray, and along line 
			// segment then intersect point is valid
			if (t1 > 0.0f && t2 >= 0.0f && t2 <= 1.0f)
			{
				// Check if this intersect point is closest to source. If
				// it is, then store this point and reject others
				if (t1 < min_t1)
				{
					min_t1 = t1;
					currentIntersectionX = lightRay.x1 + lightRayDistanceX * t1;
					currentIntersectionY = lightRay.y1 + lightRayDistanceY * t1;
					tempLineIndex = i;
				}

			}
		}
	}

	return tempLineIndex;
}

void LightMap::SortVertices(float mouseX, float mouseY)
{
	std::vector<float> vecAngles;

	//Calculating the angle for all vertices and store it in a vector
	for (Uint32 i = 0; i < vertices.size(); i++)
	{
		float x = vertices[i].x - mouseX, y = vertices[i].y - mouseY;
		float angle = atan2f(y, x);

		vecAngles.push_back(angle);
	}

	//Sorting the vertices by the angle
	for (Uint32 i = 0; i < vertices.size(); i++) //Selection sort
	{
		for (Uint32 j = 0; j < vertices.size(); j++)
		{
			if (vecAngles[j] < vecAngles[i])
			{
				float tAng = vecAngles[i];
				vecAngles[i] = vecAngles[j];
				vecAngles[j] = tAng;

				Vertex tVert = vertices[i];
				vertices[i] = vertices[j];
				vertices[j] = tVert;
			}
		}
	}

}
