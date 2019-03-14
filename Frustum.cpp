#include "Frustum.h"


Frustum::Frustum()
{
	/*this->frustumNearDist = PROJECTION_NEAR_Z;
	this->frustumFarDist = PROJECTION_FAR_Z;*/
	
	cameraReference = nullptr;
}

Frustum::~Frustum()
{
	//Nothing dynamic
}

Frustum::Frustum(Camera * camera)
{
	/*this->frustumNearDist = PROJECTION_NEAR_Z;
	this->frustumFarDist = PROJECTION_FAR_Z;*/
	cameraReference = camera;
}

Frustum::Frustum(const Frustum & original)
{
	cameraReference = original.cameraReference;

	frustumNearDist = original.frustumNearDist;
	frustumFarDist = original.frustumFarDist;
	
	frustumHightNear=original.frustumHightNear;
	frustumWidthNear=original.frustumWidthNear;

	frustumHightFar = original.frustumHightFar;
	frustumWidthFar = original.frustumWidthFar;

	//Frustum points
	frustumNearCenter = original.frustumNearCenter;
	frustumFarCenter = original.frustumFarCenter;

	//Near points
	frustNbr = original.frustNbr; //bottomright
	frustNtr = original.frustNtr; //topright
	frustNtl = original.frustNtl; //topleft
	frustNbl = original.frustNbl; //bottomleft
	//Far points
	frustFbr = original.frustFbr;
	frustFtr = original.frustFtr;
	frustFtl = original.frustFtl;
	frustFbl = original.frustFbl;

	// NEARPLANE, FARPLANE, RIGHTPLANE, TOPPLANE, LEFTPLANE, BOTTOMPLANE 
	//Frustum Normals pointed inwards of frustum
	for (int i = 0; i < 6; i++)
	{
		this->planes[i] = original.planes[i];
	}

}

Frustum &Frustum::operator=(const Frustum & originalObj)
{
	if (this != &originalObj)
	{
		cameraReference = originalObj.cameraReference;

		frustumNearDist = originalObj.frustumNearDist;
		frustumFarDist = originalObj.frustumFarDist;

		frustumHightNear = originalObj.frustumHightNear;
		frustumWidthNear = originalObj.frustumWidthNear;

		frustumHightFar = originalObj.frustumHightFar;
		frustumWidthFar = originalObj.frustumWidthFar;

		//Frustum points
		frustumNearCenter = originalObj.frustumNearCenter;
		frustumFarCenter = originalObj.frustumFarCenter;

		//Near points
		frustNbr = originalObj.frustNbr; //bottomright
		frustNtr = originalObj.frustNtr; //topright
		frustNtl = originalObj.frustNtl; //topleft
		frustNbl = originalObj.frustNbl; //bottomleft
		//Far points
		frustFbr = originalObj.frustFbr;
		frustFtr = originalObj.frustFtr;
		frustFtl = originalObj.frustFtl;
		frustFbl = originalObj.frustFbl;

		// NEARPLANE, FARPLANE, RIGHTPLANE, TOPPLANE, LEFTPLANE, BOTTOMPLANE 
		//Frustum Normals pointed inwards of frustum
		for (int i = 0; i < 6; i++)
		{
			this->planes[i] = originalObj.planes[i];
		}

	}
	return *this;
}

void Frustum::GiveCameraReference(Camera * theCamera)
{
	cameraReference = theCamera;
}

void Frustum::calculateFrustum(/*float FOV, float W_WIDTH, float W_HEIGHT*/)
{
	//NEAR H / W
	int ratio = (W_WIDTH / W_HEIGHT);
	this->frustumHightNear = 2 * tan(FOV / 2)* frustumNearDist;
	this->frustumWidthNear = frustumHightNear * /*ratioWindow*/ ratio;
	//FAR H / W
	this->frustumHightFar = 2 * tan(FOV / 2)* frustumFarDist;
	this->frustumWidthFar = frustumHightFar * ratio;

	Vector4 tempUp = cameraReference->GetCamUp();
	Vector4 tempRight = cameraReference->GetCamRight();
	Vector4 verticalEdgeFar = (tempUp*(frustumHightFar / 2));
	Vector4 horizontalEdgeFar = ((tempRight *(frustumWidthFar / 2)));
	Vector4 verticalEgeNear = (tempUp* (frustumHightNear / 2));
	Vector4 horizontalEdgeNear = (tempRight *(frustumWidthNear / 2));

	this->frustumFarCenter = cameraReference->GetCamPos() + cameraReference->GetCameraNormal() * frustumFarDist;

	this->frustFtl = frustumFarCenter + verticalEdgeFar - horizontalEdgeFar;
	this->frustFtr = frustumFarCenter + verticalEdgeFar + horizontalEdgeFar;
	this->frustFbl = frustumFarCenter - verticalEdgeFar - horizontalEdgeFar;
	this->frustFbr = frustumFarCenter - verticalEdgeFar + horizontalEdgeFar;

	//can be optimsed further.

	this->frustumNearCenter = cameraReference->GetCamPos()+ cameraReference->GetCameraNormal() * frustumNearDist;

	this->frustNtl = frustumNearCenter + verticalEgeNear - horizontalEdgeNear;
	this->frustNtr = frustumNearCenter + verticalEgeNear + horizontalEdgeNear;
	this->frustNbl = frustumNearCenter - verticalEgeNear - horizontalEdgeNear;
	this->frustNbr = frustumNearCenter - verticalEgeNear + horizontalEdgeNear;

	//PLANE DEFFINITION

	this->planes[NEARPLANE].normal = XMVector4Normalize(XMVector4Cross(frustNbl, frustNtl, frustNbr));
	this->planes[NEARPLANE].pointInPlane = &this->frustumNearCenter;

	this->planes[FARPLANE].normal = XMVector4Normalize(XMVector4Cross(frustFbr, frustFtr, frustFbl));
	this->planes[FARPLANE].pointInPlane = &this->frustumFarCenter;

	this->planes[RIGHTPLANE].normal = XMVector4Normalize(XMVector4Cross(frustNbr, frustNtr, frustFbr));
	this->planes[RIGHTPLANE].pointInPlane = &this->frustNbr;

	this->planes[TOPPLANE].normal = XMVector4Normalize(XMVector4Cross(frustFtr, frustNtr, frustFtl));
	this->planes[TOPPLANE].pointInPlane = &this->frustFtr;

	this->planes[LEFTPLANE].normal = XMVector4Normalize(XMVector4Cross(frustFbl, frustFtl, frustNbl));
	this->planes[LEFTPLANE].pointInPlane = &this->frustFbl;

	this->planes[BOTTOMPLANE].normal = XMVector4Normalize(XMVector4Cross(frustNbr, frustFbr, frustNbl));
	this->planes[BOTTOMPLANE].pointInPlane = &this->frustNbr;

}


int Frustum::pointInFrustum(Vector4 point)
{
	int result = INSIDE;

	for (int i = 0; i < 6; i++)
	{
		if (this->planes[i].distance(point) < 0)
			result = OUTSIDE;
		i = 6;
	}
	return result;
}

int Frustum::sphereInFrustum(Vector4 & point, float radius)
{
	float distance;
	int result = INSIDE;

	for (int i = 0; i < 6; i++)
	{
		distance = planes[i].distance(point);
		if (distance < -radius)
		{
			result = OUTSIDE;
			i = 6;
		}
			
		else if (distance < radius)
			result = INTERSECT;
	}

	return result;
}

int Frustum::FrustumPlanes::distance(Vector4 objectPoint)
{
	Vector4 vectorToPoint = objectPoint - *pointInPlane;

	Vector4 output = DirectX::XMVector4Dot(normal, vectorToPoint);

	return output.x;

}
