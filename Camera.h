#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#define ROTATION_GAIN 0.004f
#define MOVEMENT_GAIN 0.07f


//std::unique_ptr<DirectX::Keyboard>m_keyboard; //NOT IMPLEMENTED
//TOOL KIT
//#include "CommonStates.h"
//#include "DDSTextureLoader.h"
//#include "DirectXHelpers.h"
//#include "Effects.h"
//#include "GamePad.h"
//#include "GeometricPrimitive.h"
//#include "GraphicsMemory.h"
//#include "Keyboard.h"
//#include "Model.h"
//#include "Mouse.h"
//#include "PostProcess.h"
//#include "PrimitiveBatch.h"
//#include "ScreenGrab.h"

#include "SimpleMath.h"


using namespace DirectX;
using namespace SimpleMath;

class Camera {

private:
	//CAM DATA 
	XMVECTOR cameraPosition;
	XMVECTOR cameraOriginalPosition;
	XMVECTOR cameraTarget;
	//
	XMVECTOR cameraNormal;
	//
	XMVECTOR cameraOriginalTarget;

	//CAM MANIPULATION
	XMVECTOR const defaultForward = {0.0f,0.0f,1.0f,0.0f};
	XMVECTOR const defaultRight = { 1.0f,0.0f,0.0f,0.0f };
	XMVECTOR camForward = { 0.0f,0.0f,1.0f,0.0f };
	XMVECTOR camRight = { 1.0f,0.0f,0.0f,0.0f };
	XMVECTOR camUp = { 0.0f,1.0f,0.0f,0.0f };

	XMMATRIX camRotationMatrix;
	
	float pitch=0.0f;
	float yaw=0.0f;

public:
	Camera();
	Camera(Vector4 camPos,Vector4 camFocus);
	~Camera();

	Camera(const Camera &originalObj);

	Camera &operator= (const Camera &originalObj);

	XMVECTOR GetCamPos() const;
	XMVECTOR GetCamOriginalPos() const;
	XMVECTOR GetCamTarget()const;
	XMVECTOR GetCamOriginalTarget() const;
	XMVECTOR GetCamForward()const;
	XMVECTOR GetCamRight()const;
	XMVECTOR GetCamUp() const;

	XMVECTOR GetCameraNormal() const;

	float GetYaw()const;
	float GetPitch()const;
	void UpdateCameraNormal();

	//SETS HIGHT TO A SPECIFIK VALUE
	void SetCameraHeight(float newY);

	void SetCamPos(Vector4 position);
	void SetCamTarget(Vector4 focusPoint);

	void MoveCamPos(Vector4 move);
	void MoveCamTarget(Vector4 move);
	
	void UpdateCamera(Vector3 movement, float speedMultiplier,double time);

	void AddYaw(float rotationY);

	void AddPitch(float rotationZ);

	void UpdateYawAndPitch(float rotationY, float rotationZ);

	void SetYawAndPitch(float rotationY, float rotationZ);
		
};