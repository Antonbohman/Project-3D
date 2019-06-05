#pragma once
//#include <iostream>
//#include <string>
#include "Quadtree.h"


void Quadtree::divideTree()
{
	float halfW = this->boundary.width / 2;
	float halfH = this->boundary.height / 2;

	this->northEast = new Quadtree(RectangleData(halfW, halfH, this->boundary.xCenterPos + halfW, this->boundary.zCenterPos + halfH));
	this->northWest = new Quadtree(RectangleData(halfW, halfH, this->boundary.xCenterPos - halfW, this->boundary.zCenterPos + halfH));
	this->southWest = new Quadtree(RectangleData(halfW, halfH, this->boundary.xCenterPos - halfW, this->boundary.zCenterPos - halfH));
	this->southEast = new Quadtree(RectangleData(halfW, halfH, this->boundary.xCenterPos + halfW, this->boundary.zCenterPos - halfH));

	//insert old points
	for (int i = 0; i < nrOfElements; i++)
	{
		Point putIntoChild = *this->Points[i];


		if (this->northEast->insideRectangle(putIntoChild))
		{
			this->northEast->insert(putIntoChild);
		}
		else if (this->northWest->insideRectangle(putIntoChild))
		{
			this->northWest->insert(putIntoChild);
		}
		else if (this->southWest->insideRectangle(putIntoChild))
		{
			this->southWest->insert(putIntoChild);
		}
		else if (this->southEast->insideRectangle(putIntoChild))
		{
			this->southEast->insert(putIntoChild);
		}
		delete this->Points[i];
	}

	delete[] this->Points;
	this->capacity = 0;
	this->nrOfElements = 0;
	this->divided = true;
}

Quadtree::Quadtree()
{
	this->boundary = RectangleData();
	this->capacity = 4;
	this->Points = nullptr;

	this->nrOfElements = 0;
	this->divided = false;
	this->northEast = nullptr;
	this->northWest = nullptr;
	this->southWest = nullptr;
	this->southEast = nullptr;

}

Quadtree::Quadtree(float width, float height, float rectCenterX, float rectCenterZ)
{
	this->boundary = RectangleData(width, height, rectCenterX, rectCenterZ);
	this->capacity = 4;
	this->Points = nullptr;
	this->nrOfElements = 0;
	this->divided = false;

	this->northEast = nullptr;
	this->northWest = nullptr;
	this->southWest = nullptr;
	this->southEast = nullptr;
}

Quadtree::Quadtree(RectangleData boundary)
{
	this->boundary = boundary;
	this->capacity = 4;
	this->Points = nullptr;
	this->nrOfElements = 0;
	this->divided = false;

	this->northEast = nullptr;
	this->northWest = nullptr;
	this->southWest = nullptr;
	this->southEast = nullptr;
}

void Quadtree::search(RectangleData bounds, int &output, int *elementsOutputArray, int &capacityArray)
{
	//search for every relevant object
	if (!this->intersect(bounds))
	{
		//nothing
	}
	else
	{
		if (this->divided)
		{
			
			this->northEast->search(bounds, output, elementsOutputArray, capacityArray);

			this->northWest->search(bounds, output, elementsOutputArray, capacityArray);

			this->southWest->search(bounds, output, elementsOutputArray, capacityArray);

			this->southEast->search(bounds, output, elementsOutputArray, capacityArray);
		}
		else if (this->nrOfElements > 0)
		{
			for (int i = 0; i < this->nrOfElements; i++)
			{
				elementsOutputArray[output] = this->Points[i]->index;
				output++;
			}

		}
	}
	//return array of indexes amount is in input variable	
}

void Quadtree::searchSphere(float x, float z, float radius, int & output, int * elementsOutputArray, int & capacity)
{
	
}

bool Quadtree::intersect(RectangleData rect)
{
	return !(
		rect.xCenterPos - rect.width >
		this->boundary.xCenterPos + this->boundary.width ||
		rect.xCenterPos + rect.width <
		this->boundary.xCenterPos - this->boundary.width ||

		rect.zCenterPos - rect.height >
		this->boundary.zCenterPos + this->boundary.height ||
		rect.zCenterPos + rect.height <
		this->boundary.zCenterPos - this->boundary.height
		);
}

bool Quadtree::intersect(Point thePoint)
{
	return !(
		thePoint.x >
		this->boundary.xCenterPos + this->boundary.width ||
		thePoint.x <
		this->boundary.xCenterPos - this->boundary.width ||

		thePoint.z >
		this->boundary.zCenterPos + this->boundary.height ||
		thePoint.z <
		this->boundary.zCenterPos - this->boundary.height
		);
}


bool Quadtree::intersectCircle(float x, float z, float radious, Point thePoint)
{
	return false;
}

void Quadtree::insert(Point objectPosition)
{
	if (this->Points == nullptr)
	{
		this->Points = new Point*[this->capacity]{ nullptr };
	}

	if (this->capacity > nrOfElements)
	{
		this->Points[nrOfElements] = new Point(objectPosition.x, objectPosition.y, objectPosition.z, objectPosition.index);
		this->nrOfElements++;
	}
	else
	{
		if (this->divided == false)
		{
			divideTree();
		}

		if (this->northEast->insideRectangle(objectPosition))
		{
			this->northEast->insert(objectPosition);
		}
		else if (this->northWest->insideRectangle(objectPosition))
		{
			this->northWest->insert(objectPosition);
		}
		else if (this->southWest->insideRectangle(objectPosition))
		{
			this->southWest->insert(objectPosition);
		}
		else if (this->southEast->insideRectangle(objectPosition))
		{
			this->southEast->insert(objectPosition);
		}
	}
}

void Quadtree::insert(float x, float y,float z, float index)
{
	Point objectPosition(x, y,z, index);
	if (this->Points == nullptr)
	{
		this->Points = new Point*[this->capacity]{ nullptr };
	}

	if (this->capacity > nrOfElements)
	{
		this->Points[nrOfElements] = new Point(objectPosition.x, objectPosition.y,objectPosition.z, objectPosition.index);
		this->nrOfElements++;
	}
	else
	{
		if (this->divided == false)
		{
			divideTree();
		}

		if (this->northEast->insideRectangle(objectPosition))
		{
			this->northEast->insert(objectPosition);
		}
		else if (this->northWest->insideRectangle(objectPosition))
		{
			this->northWest->insert(objectPosition);
		}
		else if (this->southWest->insideRectangle(objectPosition))
		{
			this->southWest->insert(objectPosition);
		}
		else if (this->southEast->insideRectangle(objectPosition))
		{
			this->southEast->insert(objectPosition);
		}
	}
}

void Quadtree::insert(Point * objectReference)
{
	if (this->Points == nullptr)
	{
		this->Points = new Point*[this->capacity]{ nullptr };
	}

	if (this->capacity > nrOfElements)
	{
		this->Points[nrOfElements] = objectReference;
		this->nrOfElements++;
	}
	else
	{
		if (this->divided == false)
		{
			divideTree();
		}

		if (this->northEast->insideRectangle(*objectReference))
		{
			this->northEast->insert(objectReference);
		}
		else if (this->northWest->insideRectangle(*objectReference))
		{
			this->northWest->insert(objectReference);
		}
		else if (this->southWest->insideRectangle(*objectReference))
		{
			this->southWest->insert(objectReference);
		}
		else if (this->southEast->insideRectangle(*objectReference))
		{
			this->southEast->insert(objectReference);
		}
	}
}

bool Quadtree::insideRectangle(Point objectPos)
{
	bool returnValue = false;
	if (objectPos.x >= this->boundary.xCenterPos - this->boundary.width &&
		objectPos.x <= this->boundary.xCenterPos + this->boundary.width &&
		objectPos.z >= this->boundary.zCenterPos - this->boundary.height &&
		objectPos.z <= this->boundary.zCenterPos + this->boundary.height)
	{
		returnValue = true;
	}

	return returnValue;
}

Quadtree::~Quadtree()
{
	cleansLeaves();
}

void Quadtree::cleansLeaves()
{
	if (this->divided)
	{
		this->northEast->cleansLeaves();
		delete this->northEast;
		this->northWest->cleansLeaves();
		delete this->northWest;
		this->southWest->cleansLeaves();
		delete this->southWest;
		this->southEast->cleansLeaves();
		delete this->southEast;

		this->divided = false;

	}
	else if (this->divided == false && nrOfElements > 0)
	{
		for (int i = 0; i < this->nrOfElements; i++)
		{
			delete this->Points[i];
		}
		this->nrOfElements = 0;
		delete[] this->Points;

	}

}

Point::Point()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->index = 0;
}

Point::Point(float x, float y,float z, unsigned int index)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->index = index;
}

Point::Point(float x, float y,float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->index = 0;
}


Point::~Point()
{
	//STOCK
}

RectangleData::RectangleData()
{
	this->width = 0.5f;
	this->height = 0.5f;
	this->xCenterPos = 0;
	this->zCenterPos = 0;

}

RectangleData::RectangleData(float width, float height, float rectCenterX, float rectCenterY)
{
	this->width = width;
	this->height = height;
	this->xCenterPos = rectCenterX;
	this->zCenterPos = rectCenterY;
}

RectangleData::~RectangleData()
{
	//Non dynamic
}
