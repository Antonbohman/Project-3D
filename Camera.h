
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
	XMVECTOR cameraOriginalFocus;

	//CAM MANIPULATION
	XMVECTOR const defaultForward = {0.0f,0.0f,1.0f,0.0f};
	XMVECTOR const defaultRight = { 1.0f,0.0f,0.0f,0.0f };
	XMVECTOR camForward = { 0.0f,0.0f,1.0f,0.0f };
	XMVECTOR camRight = { 1.0f,0.0f,0.0f,0.0f };
	XMVECTOR camUp = { 0.0f,1.0f,0.0f,0.0f };
	
	float pitch;
	float yaw;

public:
	Camera();
	Camera(Vector4 camPos,Vector4 camFocus);
	~Camera();

	Camera(const Camera &orignalObj);

	Camera &operator= (const Camera &originalObj);

	XMVECTOR getCamPos() const;
	XMVECTOR getCamOriginalPos() const;
	XMVECTOR getCamTarget()const;
	XMVECTOR getCamOriginalFocus() const;
	XMVECTOR getCamForward()const;
	XMVECTOR getCamRight()const;
	XMVECTOR getCamUp() const;

	float getYaw()const;
	float getPitch()const;

	void setCamPos(Vector4 position);
	void setCamTarget(Vector4 focusPoint);

	void moveCamPos(Vector4 move);
	void moveCamTarget(Vector4 move);

	void addYaw(float rotationY);

	void addPitch(float rotationZ);

	void updateYawAndPitch(float rotationY, float rotationZ);
		
};