#pragma once

#include "Frustum.h"

enum{NE,NW,SW,SE};
//++,-+,--,+-

class Quadtree
{
	private:

	int mapWidth;
	int mapHeight;

	unsigned int content;

	struct Quad
	{
		Vector2 Quadrant[4];
		Quad* parent;
		Quad* children[4];

	};
	Quad* rot;

	void Divide();

public:
	
	Quadtree(int mapWidth,int mapHeight);

	~Quadtree();//later

	
};