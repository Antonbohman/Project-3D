#include "Camera.h"


Camera::Camera()
{
	cameraPosition = { 0.0f,0.0f,-10.0f,0.0f };
	cameraOriginalPosition = cameraPosition;
	cameraTarget = { 0.0f,0.0f,0.0f,0.0f };
	cameraNormal = XMVector3Normalize(cameraTarget - cameraPosition);
	cameraOriginalTarget = cameraTarget;

}

Camera::Camera(Vector4 camPos, Vector4 camFocus)
{
	cameraPosition = camPos;
	cameraOriginalPosition = cameraPosition;
	cameraTarget = camFocus;
	cameraNormal = XMVector3Normalize(cameraTarget - cameraPosition);
	cameraOriginalTarget = cameraTarget;
}

Camera::~Camera()
{
	//STATIC
}

Camera::Camera(const Camera & originalObj)
{
	//CAM DATA
	this->cameraPosition=originalObj.cameraPosition;
	this->cameraOriginalPosition =originalObj.cameraOriginalPosition;
	this->cameraTarget = originalObj.cameraTarget;
	this->cameraNormal = originalObj.cameraNormal;
	this->cameraOriginalTarget = originalObj.cameraOriginalTarget;

	//CAM MANIPULATION
	// defaultForward  //STATIC
	// defaultRight  //STATIC
	this->camForward = originalObj.camForward;
	this->camRight = originalObj.camRight;
	this->camUp = originalObj.camUp;

	this->pitch=originalObj.pitch;
	this->yaw=originalObj.yaw;
}

Camera & Camera::operator=(const Camera & originalObj)
{
	if (this != &originalObj)
	{
		//CAM DATA
		this->cameraPosition = originalObj.cameraPosition;
		this->cameraOriginalPosition = originalObj.cameraOriginalPosition;
		this->cameraTarget = originalObj.cameraTarget;
		this->cameraNormal = originalObj.cameraNormal;
		this->cameraOriginalTarget = originalObj.cameraOriginalTarget;

		//CAM MANIPULATION
		//defaultForward  //STATIC
		//defaultRight  //STATIC
		this->camForward = originalObj.camForward;
		this->camRight = originalObj.camRight;
		this->camUp = originalObj.camUp;

		this->pitch = originalObj.pitch;
		this->yaw = originalObj.yaw;

	}
	return *this;
	// TODO: insert return statement here
}

XMVECTOR Camera::GetCamPos()const
{
	return cameraPosition;
}

XMVECTOR Camera::GetCamOriginalPos()const
{
	return cameraOriginalPosition;
}

XMVECTOR Camera::GetCamTarget()const
{
	return cameraTarget;
}

XMVECTOR Camera::GetCamOriginalTarget()const
{
	return cameraOriginalTarget;
}

XMVECTOR Camera::GetCamForward()const
{
	return camForward;
}

XMVECTOR Camera::GetCamRight()const
{
	return camRight ;
}

XMVECTOR Camera::GetCamUp() const
{
	return camUp;
}

XMVECTOR Camera::GetCameraNormal() const
{
	return this->cameraNormal;
}

void Camera::calculateFrustum(float FOV, float W_WIDTH, float W_HEIGHT)
{
	//NEAR H / W
	int ratio = (W_WIDTH / W_HEIGHT);
	this->frustumHightNear = 2 * tan(FOV / 2)* frustumNearDist;
	this->frustumWidthNear = frustumHightNear * /*ratioWindow*/ ratio;
	//FAR H / W
	this->frustumHightFar = 2 * tan(FOV / 2)* frustumFarDist;
	this->frustumWidthFar = frustumHightFar * ratio;

	Vector4 tempUp = camUp;
	Vector4 tempRight = camRight;
	this->frustumFarCenter = cameraPosition + cameraNormal * frustumFarDist;
	this->frustFtl = frustumFarCenter + (tempUp* (frustumHightFar / 2)) - (tempRight *(frustumWidthFar / 2));
	this->frustFtr = frustumFarCenter + (tempUp* (frustumHightFar / 2)) + (tempRight *(frustumWidthFar / 2));
	this->frustFbl = frustumFarCenter - (tempUp* (frustumHightFar / 2)) - (tempRight *(frustumWidthFar / 2));
	this->frustFbr = frustumFarCenter - (tempUp* (frustumHightFar / 2)) + (tempRight *(frustumWidthFar / 2));

	this->frustumNearCenter = cameraPosition + cameraNormal * frustumNearDist;

	this->frustNtl = frustumNearCenter + (tempUp* (frustumHightNear / 2)) - (tempRight *(frustumWidthNear / 2));
	this->frustNtr = frustumNearCenter + (tempUp* (frustumHightNear / 2)) - (tempRight *(frustumWidthNear / 2));
	this->frustNbl = frustumNearCenter + (tempUp* (frustumHightNear / 2)) - (tempRight *(frustumWidthNear / 2));
	this->frustNbr = frustumNearCenter + (tempUp* (frustumHightNear / 2)) - (tempRight *(frustumWidthNear / 2));

	//PLANE DEFFINITION WASASAASASASASASASASSAASASSASAASASSASAASAAAAAA

}

float Camera::GetYaw() const
{
	return this->yaw;
}

float Camera::GetPitch() const
{
	return this->pitch;
}

void Camera::UpdateCameraNormal()
{
	this->cameraNormal = XMVector4Normalize(this->cameraTarget - this->cameraPosition);
}

void Camera::SetCamPos(Vector4 position)
{
	this->cameraPosition = position;
}

void Camera::SetCamTarget(Vector4 focusPoint)
{
	this->cameraTarget = focusPoint;
}

void Camera::MoveCamPos(Vector4 move)
{
	cameraPosition += move;
}

void Camera::MoveCamTarget(Vector4 move)
{
	cameraTarget += move;
}

void Camera::UpdateCamera(Vector3 movement, double time)
{

	//ROTATION OF CAMERA
	this->camRotationMatrix = XMMatrixRotationRollPitchYaw(this->pitch, this->yaw, 0);
	//cameraFocus = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	//cameraFocus = XMVector3Normalize(cameraFocus); //DID NOT WORK

	//DID WORK
	float y = sinf(pitch);
	float r = cosf(pitch);
	float z = r * cosf(yaw);
	float x = r * sinf(yaw);
	this->cameraTarget = this->cameraPosition + XMVECTOR{ x, y, z, 0.0f };
	//cameraFocus = XMVector3Normalize(cameraFocus); 

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(yaw);

	//Update cameraForward,Up,Right
	camRight = XMVector3TransformCoord(defaultRight, RotateYTempMatrix);
	camRight = XMVector3Normalize(camRight);
	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(defaultForward, RotateYTempMatrix);

	Vector4 timeMovement = XMVector3Normalize(movement);
	timeMovement = timeMovement* time;

	MoveCamTarget(timeMovement);
	MoveCamPos(timeMovement);

	UpdateCameraNormal();
}

void Camera::AddYaw(float rotationY)
{
	yaw -= rotationY;
	if (yaw > XM_PI)
	{
		yaw -= XM_PI * 2.0f;
	}
	else if (yaw < -XM_PI)
	{
		yaw += XM_PI * 2.0f;
	}
}

void Camera::AddPitch(float rotationZ)
{
	this->pitch -= rotationZ;

	float limit = XM_PI / 2.0f - 0.01f;
	pitch = max(-limit, pitch); //sets maximum value
	pitch = min(+limit, pitch); //set minimum value
}

void Camera::UpdateYawAndPitch(float addYaw, float addPitch)
{
	this->pitch -= addPitch;
	this->yaw -= addYaw;
	

	if (yaw > XM_PI)
	{
		yaw -= XM_PI * 2.0f;
	}
	else if (yaw < -XM_PI)
	{
		yaw += XM_PI * 2.0f;
	}

	float limit = XM_PI / 2.0f - 0.1f;
	pitch = max(-limit, pitch); //sets maximum value
	pitch = min(+limit, pitch); //set minimum value

}

void Camera::SetYawAndPitch(float rotationY, float rotationZ)
{
	yaw = rotationY;
	if (yaw > XM_PI)
	{
		yaw -= XM_PI * 2.0f;
	}
	else if (yaw < -XM_PI)
	{
		yaw += XM_PI * 2.0f;
	}

	this->pitch = rotationZ;

	float limit = XM_PI / 2.0f - 0.1f;
	pitch = max(-limit, pitch); //sets maximum value
	pitch = min(+limit, pitch); //set minimum value
}


