#pragma once
//#include <d3d11.h>
//#include <d3dcompiler.h>
//#include <DirectXMath.h>

#include "Camera.h"
#include "Quadtree.h"


class Frustum {
public:
	Camera* cameraReference;

	//http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
	enum InsideFrustum { OUTSIDE, INTERSECT, INSIDE };
	
	//Frustum data Accelerator
	float frustumNearDist;
	float frustumFarDist;

	float frustumHightNear;
	float frustumWidthNear;

	float frustumHightFar;
	float frustumWidthFar;

	float scaleFactor;
	
	//Frustum points
	Vector4 frustumNearCenter;
	Vector4 frustumFarCenter;

	//Near points
	Vector4 frustNbr; //bottomright
	Vector4 frustNtr; //topright
	Vector4 frustNtl; //topleft
	Vector4 frustNbl; //bottomleft
	//Far points
	Vector4 frustFbr;
	Vector4 frustFtr;
	Vector4 frustFtl;
	Vector4 frustFbl;

	//Frustum "middle" for a ok point at search
	Vector4 frustMiddle;
	Vector4 cameraPos;

	enum PlaneNames{
		NEARPLANE,
		FARPLANE,
		RIGHTPLANE,
		TOPPLANE,
		LEFTPLANE,
		BOTTOMPLANE};
	//Frustum Normals pointed inwards of frustum

	struct FrustumPlanes
	{
		Vector4 normal;
		Vector4* pointInPlane;

		int distance(Vector3 objectPoint);
	};
	FrustumPlanes planes[6];

	Frustum();

	~Frustum();

	Frustum(Camera* camera);

	Frustum(const Frustum &original);

	Frustum &operator= (const Frustum &originalObj);

	void GiveCameraReference(Camera * theCamera);

	void calculateFrustum(float fov, float windowWidth, float windowHeight);

	void calculateFrustum(float fov, float windowWidth, float windowHeight, float scaleFactor);

	int pointInFrustum(Vector3 point);

	//CHECKS ONLY with 2 planes
	int pointInFrustum(Vector3 point, float extraRange);

	bool insideChild(Quadtree* theTree,float extraDiscrepancy);

	int isQuadInside(Quadtree* theTree, int & output, int * elementsOutputArray, int & capacity);

	int sphereInFrustum(Vector3 point, float radius);

};