#include "Frustum.h"
#include "Global.h"

Frustum::Frustum()
{
	this->frustumNearDist = PROJECTION_NEAR_Z;
	this->frustumFarDist = PROJECTION_FAR_Z ;
	this->scaleFactor = 1.0f;
	
	cameraReference = nullptr;
}

Frustum::~Frustum()
{
	//Nothing dynamic
}

Frustum::Frustum(Camera * camera)
{
	this->frustumNearDist = PROJECTION_NEAR_Z;
	this->frustumFarDist = PROJECTION_FAR_Z ;
	this->scaleFactor = 1.0f;
	cameraReference = camera;
}

Frustum::Frustum(const Frustum & original)
{

	cameraReference = original.cameraReference;
	this->scaleFactor = original.scaleFactor;

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
		this->scaleFactor = originalObj.scaleFactor;

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

void Frustum::calculateFrustum(float fov, float windowWidth, float windowHeight)
{
	float scale = this->scaleFactor;
	//float scale = 1;
	//NEAR H / W
	float ratio = (windowWidth/ windowHeight );
	//ratio = 16.0f / 9.0f;
	this->frustumHightNear = tan(((fov) * XM_PI)/1.5)* frustumNearDist;
	this->frustumWidthNear = frustumHightNear *ratio;
	//FAR H / W
	this->frustumHightFar = tan(((fov ) * XM_PI)/1.5)* frustumFarDist;
	this->frustumWidthFar = frustumHightFar * ratio;

	Vector4 tempUp = cameraReference->GetCamUp();
	Vector4 tempRight = cameraReference->GetCamRight();

	Vector4 verticalEdgeFar = scale*(tempUp*(frustumHightFar / 2 ));

	Vector4 horizontalEdgeFar = scale * ((tempRight*(frustumWidthFar / 2 )));

	Vector4 verticalEdgeNear = scale * (tempUp* (frustumHightNear / 2));

	Vector4 horizontalEdgeNear = scale * (tempRight *(frustumWidthNear / 2));

	this->frustumFarCenter = cameraReference->GetCamPos() + cameraReference->GetCameraNormal() * (frustumFarDist);

	this->frustMiddle = cameraReference->GetCamPos() + cameraReference->GetCameraNormal() * (frustumFarDist) /2;

	this->cameraPos = cameraReference->GetCamPos();

	this->frustFtl = frustumFarCenter + verticalEdgeFar - horizontalEdgeFar;
	this->frustFtr = frustumFarCenter + verticalEdgeFar + horizontalEdgeFar;
	this->frustFbl = frustumFarCenter - verticalEdgeFar - horizontalEdgeFar;
	this->frustFbr = frustumFarCenter - verticalEdgeFar + horizontalEdgeFar;

	//can be optimsed further.

	this->frustumNearCenter = cameraReference->GetCamPos()+ cameraReference->GetCameraNormal() * frustumNearDist;;

	this->frustNtl = frustumNearCenter + verticalEdgeNear - horizontalEdgeNear;
	this->frustNtr = frustumNearCenter + verticalEdgeNear + horizontalEdgeNear;
	this->frustNbl = frustumNearCenter - verticalEdgeNear - horizontalEdgeNear;
	this->frustNbr = frustumNearCenter - verticalEdgeNear + horizontalEdgeNear;

	//PLANE DEFFINITION

//	this->planes[NEARPLANE].normal = XMVector3Normalize(XMVector3Cross((frustNtl-frustNbl),(frustNbr - frustNbl)));

	this->planes[NEARPLANE].normal = XMVector3Normalize(this->cameraReference->GetCamForward())/*XMVector3Normalize(XMVector3Cross((frustNbr - frustNbl), (frustNtl - frustNbl)))*/;
	this->planes[NEARPLANE].pointInPlane = &this->frustumNearCenter;


	this->planes[FARPLANE].normal = XMVector3Normalize(XMVector3Cross((frustFbl - frustFbr),(frustFtr - frustFbr) ));
	this->planes[FARPLANE].pointInPlane = &this->frustumFarCenter;

	
	this->planes[RIGHTPLANE].normal = XMVector3Normalize(XMVector3Cross((frustNbr - frustNtr), (frustFtr - frustNtr)));
	this->planes[RIGHTPLANE].pointInPlane = &this->frustNtr;

	
	this->planes[TOPPLANE].normal = XMVector3Normalize(XMVector3Cross((frustFtr - frustNtr), (frustNtl - frustNtr)));
	this->planes[TOPPLANE].pointInPlane = &this->frustFtr;

	this->planes[LEFTPLANE].normal = XMVector3Normalize(XMVector3Cross((frustFbl - frustFtl), (frustNtl - frustFtl)));
	this->planes[LEFTPLANE].pointInPlane = &this->frustNtl;

	
	this->planes[BOTTOMPLANE].normal = XMVector3Normalize(XMVector3Cross((frustNbl - frustNbr), (frustFbr - frustNbr)));
	this->planes[BOTTOMPLANE].pointInPlane = &this->frustNbr;
}

void Frustum::calculateFrustum(float fov, float windowWidth, float windowHeight, float scaleFactor)
{
	this->scaleFactor = scaleFactor;
	this->calculateFrustum(fov, windowWidth, windowHeight);
}

int Frustum::pointInFrustum(Vector3 point)
{
	int result = INSIDE;

	for (int i = 0; i < 6; i++)
	{
		if (this->planes[i].distance(point) < 0)
		{
			result = OUTSIDE;
			i = 6;
		}
	}
	return result;
}

int Frustum::pointInFrustum(Vector3 point, float extraRange)
{
	/*enum PlaneNames {
		NEARPLANE,//removed
		FARPLANE,
		RIGHTPLANE,
		TOPPLANE,
		LEFTPLANE,
		BOTTOMPLANE
	};*/
	float distance;
	int result = INSIDE;

	for (int i = 0; i < 6; i++)
	{
		if(i==RIGHTPLANE || i==LEFTPLANE )
		{
			distance = this->planes[i].distance(point);

			if (distance < 0)
			{
				if (distance + extraRange < 0)
				{
					result = OUTSIDE;
					i = 6;
				}
			}
		}
	}
	return result;
}

bool Frustum::insideChild(Quadtree * theTree, float extraDiscrepancy)
{
	bool inside = false;
	Vector3 testPoint = { theTree->boundary.xCenterPos,frustMiddle.y,theTree->boundary.zCenterPos };
	//frustum within simple checking 2 sides LEFT AND RIGHT

		//CAMERAPOS WITHIN QUAD
	if (theTree->intersect(Point(this->cameraPos.x, this->frustMiddle.y, this->cameraPos.z)))inside=true;


		//CENTER OF QUAD
	if (inside == 0)
	{
		if (pointInFrustum(testPoint, extraDiscrepancy) > 0)
		{
			inside = true;
		}
	}
	if (inside == 0)
	{
		//NE
		testPoint.x = { theTree->boundary.xCenterPos + theTree->boundary.width };
		testPoint.z = { theTree->boundary.zCenterPos + theTree->boundary.height };
		if (pointInFrustum(testPoint, extraDiscrepancy) > 0)
		{
			inside = true;
		}

	}
	if (inside == 0)
	{
		//NW

		testPoint.x = { theTree->boundary.xCenterPos - theTree->boundary.width };
		testPoint.z = { theTree->boundary.zCenterPos + theTree->boundary.height };
		if (pointInFrustum(testPoint, extraDiscrepancy) > 0)
		{
			inside = true;
		}
	}
	

	if (inside == 0)
	{
		//SE
		testPoint.x = { theTree->boundary.xCenterPos - theTree->boundary.width };
		testPoint.z = { theTree->boundary.zCenterPos - theTree->boundary.height };
		if (pointInFrustum(testPoint, extraDiscrepancy) > 0)
		{
			inside = true;
		}
	}
	if (inside == 0)
	{
		//SW
		testPoint.x = { theTree->boundary.xCenterPos + theTree->boundary.width };
		testPoint.z = { theTree->boundary.zCenterPos - theTree->boundary.height };

		if (pointInFrustum(testPoint, extraDiscrepancy) > 0)
		{
			inside = true;
		}
	}

	return inside;
}

int Frustum::isQuadInside(Quadtree* theTree, int & output, int * elementsOutputArray, int & capacity)
{
	
	int test = 0;

	float extraDiscrepancy = theTree->boundary.height / 2; 
	//extra quarter range
	

	if (theTree->divided == true)
	{
		if (insideChild(theTree->northEast, extraDiscrepancy))
		{
			this->isQuadInside(theTree->northEast, output, elementsOutputArray, capacity);
		}

		if (insideChild(theTree->northWest, extraDiscrepancy))
		{
			this->isQuadInside(theTree->northWest, output, elementsOutputArray, capacity);
		}

		if (insideChild(theTree->southWest, extraDiscrepancy))
		{
			this->isQuadInside(theTree->southWest, output, elementsOutputArray, capacity);
		}

		if (insideChild(theTree->southEast, extraDiscrepancy))
		{
			this->isQuadInside(theTree->southEast, output, elementsOutputArray, capacity);
		}

	}
	else if (theTree->nrOfElements > 0)
	{
		for (int i = 0; i < theTree->nrOfElements; i++)
		{
			elementsOutputArray[output] = theTree->Points[i]->index;
			output++;
		}

	}

	return test;
}

int Frustum::sphereInFrustum(Vector3 point, float radius)
{
	float distance;
	int result = INSIDE;

	/*enum PlaneNames {
		NEARPLANE,//removed
		FARPLANE,
		RIGHTPLANE,
		TOPPLANE,
		LEFTPLANE,
		BOTTOMPLANE
	};*/

	for (int i = 0; i < 6; i++)
	{
		if (i == NEARPLANE || i == FARPLANE || i == RIGHTPLANE || i == LEFTPLANE || i == TOPPLANE || i == BOTTOMPLANE)
		{ 
			distance = this->planes[i].distance(point);

			if (distance < 0)
			{
				if (distance + radius < 0)
				{
					result = OUTSIDE;
					i = 6;
				}
			}
		}
		
	}

	return result;
}

int Frustum::FrustumPlanes::distance(Vector3 objectPoint)
{
	Vector3 vectorToPoint = objectPoint - (Vector3{ this->pointInPlane->x,this->pointInPlane->y,this->pointInPlane->z });
	Vector3 tempNormal = { this->normal.x,this->normal.y,this->normal.z };

	Vector3 output = DirectX::XMVector3Dot(tempNormal,vectorToPoint);

	return output.x;

}
