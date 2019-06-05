#pragma once
#include <vector>
//#include "Frustum.h"

enum { NE, NW, SW, SE };
//++,-+,--,+-

class Point {
public:
	float x;
	float y;
	float z;
	unsigned int index;

	//FOR TESTING
	Point();
	//FOR TESTING
	Point(float x, float y,float z);

	Point(float x, float y,float z, unsigned int index);
	~Point();
};
class RectangleData {
public:
	float width;
	float height;
	float xCenterPos;
	float zCenterPos;

	RectangleData();
	RectangleData(float width, float height, float rectCenterX, float rectCenterZ);
	~RectangleData();
};

class Quadtree
{

private:
	void divideTree();
	void cleansLeaves();

public:


	RectangleData boundary;
	unsigned int capacity;
	Point** Points;
	unsigned int nrOfElements;
	bool divided;

	Quadtree* northEast;
	Quadtree* northWest;
	Quadtree* southWest;
	Quadtree* southEast;

	Quadtree();

	Quadtree(float width, float height, float rectCenterX, float rectCenterY);
	Quadtree(RectangleData boundary);

	void search(RectangleData bounds, int &output, int *elementsOutputArray, int &capacityArray);

	void searchSphere(float x, float z, float radious, int &output, int *elementsOutputArray, int &capacity);

	/*void search(Frustum theFrustum, int &output, int *elementsOutputArray, int &capacity);*/

	//int intersect(Frustum theFrustum);

	bool intersect(RectangleData rect);

	bool intersect(Point thePoint);



	bool intersectCircle(float x, float z, float radious, Point thePoint);

	void insert(Point objectPosition);

	void insert(float x, float y,float z, float index);

	void insert(Point* objectReference);

	bool insideRectangle(Point objectPos);

	~Quadtree();




};