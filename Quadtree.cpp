#include "Quadtree.h"



void Quadtree::Divide()
{
}

Quadtree::Quadtree(int mapWidth, int mapHeight)
{
	this->mapHeight = mapHeight;
	this->mapWidth = mapWidth;

	this->rot = nullptr;
}

Quadtree::~Quadtree()
{

	if (this->rot == nullptr)
	{
		//do nothing
	}
	else //GO DEEPER
	{
		//go deppest then upp 4 times
	}

	//else
}
