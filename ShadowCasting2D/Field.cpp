#include"Field.h"

Field::Field(int S_W, int S_H, std::vector<Line>* vecLines)
{
	SetupWindowRect(vecLines);

	SetRect(0.0f, 0.0f, vecLines);
	SetRect(-0.4f, 0.7f, vecLines);
	SetRect(0.7f, 0.0f, vecLines);


}

Field::~Field()
{
}

void Field::SetRect(float posX, float posY, std::vector<Line>* vecLines)
{
	float w = 0.08f;
	float h = 0.15f;

	posX -= w / 2;
	posY -= h / 2;

	Uint32 verticesCount = vertices.size();

	vertices.push_back({ posX, posY,
		0.0f,0.0f,0.0f,1.0f });
	vertices.push_back({ posX + w, posY,
		0.0f,0.0f,0.0f,1.0f });
	vertices.push_back({ posX, posY + h,
		0.0f,0.0f,0.0f,1.0f });
	vertices.push_back({ posX + w, posY + h,
		0.0f,0.0f,0.0f,1.0f });

	indices.push_back(vertices.size() - 4);
	indices.push_back(vertices.size() - 3);
	indices.push_back(vertices.size() - 2);
	indices.push_back(vertices.size() - 3);
	indices.push_back(vertices.size() - 2);
	indices.push_back(vertices.size() - 1);

	vecLines->push_back({ posX + w, posY, posX + w, posY + h });
	vecLines->push_back({ posX, posY + h, posX + w, posY + h });
	vecLines->push_back({ posX, posY, posX + w, posY });
	vecLines->push_back({ posX, posY, posX, posY + h });


	verticesWLight = vertices;
	IntersectAllLines(*vecLines);

}

void Field::IntersectAllLines(std::vector<Line> vecLines)
{
	for (Uint32 j = 0; j < vecLines.size(); j++)
	{
		float edgeDistanceX1 = vecLines[j].x2 - vecLines[j].x1;
		float edgeDistanceY1 = vecLines[j].y2 - vecLines[j].y1;

		for (Uint32 i = 0; i < vecLines.size(); i++)
		{
			if (i == j) 
			{
				i += 3;
			}
			else
			{
				float edgeDistanceX2 = vecLines[i].x2 - vecLines[i].x1;
				float edgeDistanceY2 = vecLines[i].y2 - vecLines[i].y1;


				if (fabs(edgeDistanceX2 - edgeDistanceX1) > 0.0f && fabs(edgeDistanceY2 - edgeDistanceY1) > 0.0f)
				{
					// t2 is normalised distance from line segment start to line segment end of intersect point
					float t2 = (edgeDistanceX1 * (vecLines[i].y1 - vecLines[j].y1) + (edgeDistanceY1 * (vecLines[j].x1 - vecLines[i].x1))) / (edgeDistanceX2 * edgeDistanceY1 - edgeDistanceY2 * edgeDistanceX1);
					// t1 is normalised distance from source along ray to ray length of intersect point
					float t1;
					if (edgeDistanceY1 == 0.0f)
						t1 = (vecLines[i].x1 + edgeDistanceX2 * t2 - vecLines[j].x1) / edgeDistanceX1;
					else
						t1 = (vecLines[i].y1 + edgeDistanceY2 * t2 - vecLines[j].y1) / edgeDistanceY1;

					// If intersect point exists along both lines, then intersect point is valid
					if (t1 >= 0.0f && t1 <= 1.0f && t2 >= 0.0f && t2 <= 1.0f)
					{
						// Check if this intersect point is closest to source. If
						// it is, then store this point and reject others
						float currentIntersectionX = vecLines[j].x1 + edgeDistanceX1 * t1;
						float currentIntersectionY = vecLines[j].y1 + edgeDistanceY1 * t1;

						//vertices.push_back({ currentIntersectionX, currentIntersectionY,
						//	1.0f, 1.0f, 1.0f, 1.0f });

						verticesWLight.push_back({ currentIntersectionX, currentIntersectionY,
							1.0f, 1.0f, 1.0f, 1.0f });
					}
				}
			}
		}
	}
}

void Field::DeleteRect(float mouseX, float mouseY, std::vector<Line>* vecLines)
{

	if (vertices.size() % 4 == 0 && vecLines->size() % 4 == 0)
	{
		for (Uint32 i = 4; i < vertices.size(); i += 4)
		{
			if (mouseX > vertices[i].x && mouseX < vertices[(i + 1)].x
				&& mouseY > vertices[i].y && mouseY < vertices[(i + 2)].y)
			{
				vertices.erase(vertices.begin() + i, vertices.begin() + (i + 4));
				vecLines->erase(vecLines->begin() + i, vecLines->begin() + (i + 4));

				for (Uint8 j = 0; j < 6; j++)
					indices.pop_back();

			}
		}
	}

	verticesWLight = vertices;
	IntersectAllLines(*vecLines);
}

void Field::SetupWindowRect(std::vector<Line>* vecLines)
{
	vertices.push_back({ -1.0f, -1.0f,
		0.0f,0.0f,0.0f,1.0f });
	vertices.push_back({ 1.0f, -1.0f,
		0.0f,0.0f,0.0f,1.0f });
	vertices.push_back({ 1.0f, 1.0f,
		0.0f,0.0f,0.0f,1.0f });
	vertices.push_back({ -1.0f, 1.0f,
		0.0f,0.0f,0.0f,1.0f });

	vecLines->push_back({ -1.0f, -1.0f, 1.0f, -1.0f });
	vecLines->push_back({ 1.0f, -1.0f, 1.0f, 1.0f });
	vecLines->push_back({ 1.0f, 1.0f, -1.0f, 1.0f });
	vecLines->push_back({ -1.0f, 1.0f, -1.0f, -1.0f });
}
