#include "Camera.h"


Camera::Camera()
{
	cameraPosition = { 0.0f,0.0f,-10.0f,0.0f };
	cameraOriginalPosition = cameraPosition;
	cameraTarget = { 0.0f,0.0f,0.0f,0.0f };
	cameraOriginalFocus = cameraTarget;

}

Camera::Camera(Vector4 camPos, Vector4 camFocus)
{
	cameraPosition = camPos;
	cameraOriginalPosition = cameraPosition;
	cameraTarget = camFocus;
	cameraOriginalFocus = cameraTarget;
}

Camera::~Camera()
{
	//STATIC
}

Camera::Camera(const Camera & orignalObj)
{
	//CAM DATA
	this->cameraPosition=orignalObj.cameraPosition;
	this->cameraOriginalPosition =orignalObj.cameraOriginalPosition;
	this->cameraTarget = orignalObj.cameraTarget;
	this->cameraOriginalFocus = orignalObj.cameraOriginalFocus;

	//CAM MANIPULATION
	// defaultForward  //STATIC
	// defaultRight  //STATIC
	this->camForward = { 0.0f,0.0f,1.0f,0.0f };
	this->camRight = { 1.0f,0.0f,0.0f,0.0f };
	this->camUp = { 0.0f,1.0f,0.0f,0.0f };

	this->pitch=orignalObj.pitch;
	this->yaw=orignalObj.yaw;
}

Camera & Camera::operator=(const Camera & originalObj)
{
	if (this != &originalObj)
	{
		//CAM DATA
		this->cameraPosition = originalObj.cameraPosition;
		this->cameraOriginalPosition = originalObj.cameraOriginalPosition;
		this->cameraTarget = originalObj.cameraTarget;
		this->cameraOriginalFocus = originalObj.cameraOriginalFocus;

		//CAM MANIPULATION
		// defaultForward  //STATIC
		// defaultRight  //STATIC
		this->camForward = { 0.0f,0.0f,1.0f,0.0f };
		this->camRight = { 1.0f,0.0f,0.0f,0.0f };
		this->camUp = { 0.0f,1.0f,0.0f,0.0f };

		this->pitch = originalObj.pitch;
		this->yaw = originalObj.yaw;

	}
	return *this;
	// TODO: insert return statement here
}

XMVECTOR Camera::getCamPos()const
{
	return cameraPosition;
}

XMVECTOR Camera::getCamOriginalPos()const
{
	return cameraOriginalPosition;
}

XMVECTOR Camera::getCamTarget()const
{
	return cameraTarget;
}

XMVECTOR Camera::getCamOriginalFocus()const
{
	return cameraOriginalFocus;
}

XMVECTOR Camera::getCamForward()const
{
	return camForward;
}

XMVECTOR Camera::getCamRight()const
{
	return camRight ;
}

XMVECTOR Camera::getCamUp() const
{
	return camUp;
}

float Camera::getYaw() const
{
	return this->yaw;
}

float Camera::getPitch() const
{
	return this->pitch;
}

void Camera::setCamPos(Vector4 position)
{
	this->cameraPosition = position;
}

void Camera::setCamTarget(Vector4 focusPoint)
{
	this->cameraTarget = focusPoint;
}

void Camera::moveCamPos(Vector4 move)
{
	cameraPosition += move;
	
}

void Camera::moveCamTarget(Vector4 move)
{
	cameraTarget += move;
}

void Camera::addYaw(float rotationY)
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

void Camera::addPitch(float rotationZ)
{
	this->pitch -= rotationZ;

	float limit = XM_PI / 2.0f - 0.01f;
	pitch = max(-limit, pitch); //sets maximum value
	pitch = min(+limit, pitch); //set minimum value
}

void Camera::updateYawAndPitch(float rotationY, float rotationZ)
{
	addYaw(rotationY);
	addPitch(rotationZ);
/*
	float y = sinf(pitch);
	float r = cosf(pitch);
	float z = r * cosf(yaw);
	float x = r * sinf(yaw);*/
	float r = cosf(getPitch());
	
	Vector3 xyz = { r*sinf(yaw),r*cosf(yaw),sinf(pitch) };
	this->cameraTarget = this->cameraPosition + XMVECTOR{xyz.x,xyz.y,xyz.z,0.0f};
}

