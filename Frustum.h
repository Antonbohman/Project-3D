#pragma once
//#include <d3d11.h>
//#include <d3dcompiler.h>
//#include <DirectXMath.h>

#include "camera.h"

class Frustum {
private:
	Camera* cameraReference;

	//http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
	enum InsideFrustum { OUTSIDE, INTERSECT, INSIDE };
	
	//Frustum data Accelerator
	float frustumNearDist=10;
	float frustumFarDist=200;

	float frustumHightNear;
	float frustumWidthNear;

	float frustumHightFar;
	float frustumWidthFar;
	
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

	enum PlaneNames{NEARPLANE,
		FARPLANE,
		RIGHTPLANE,
		TOPPLANE,
		LEFTPLANE,
		BOTTOMPLANE};
	//Frustum Normals pointed inwards of frustum

	struct FrustumPlanes
	{
	public:
		Vector4 normal;
		Vector4* pointInPlane;

		int distance(Vector4 objectPoint);
	};
	FrustumPlanes Planes[6];
	
public:

	Frustum();

	~Frustum();

	Frustum(Camera* camera);

	Frustum(const Frustum &original);

	Frustum &operator= (const Frustum &originalObj);

	void GiveCameraReference(Camera * theCamera);

	int Distance(const Vector4 &p);

	//int isInFront(Vector3 point);

	void calculateFrustum(float FOV, float W_WIDTH, float W_HEIGHT);

	int pointInFrustum(Vector3 point);
};