#include "ConstantBuffers.h"

void CreateConstantBuffer() {
	/////////////////
	// Camera	   //
	///////////////// 

	//allocate space in memory aligned to a multitude of 16
	gCameraMatrix = (CameraMatrix*)_aligned_malloc(sizeof(CameraMatrix), 16);

	//create a description objekt defining how the buffer should be handled
	D3D11_BUFFER_DESC cameraDesc;
	ZeroMemory(&cameraDesc, sizeof(cameraDesc));
	cameraDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraDesc.ByteWidth = sizeof(CameraMatrix);
	cameraDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//bind light data to a subresource
	D3D11_SUBRESOURCE_DATA cameraData;
	ZeroMemory(&cameraData, sizeof(cameraData));
	cameraData.pSysMem = gCameraMatrix;
	cameraData.SysMemPitch = 0;
	cameraData.SysMemSlicePitch = 0;

	//create buffer for light subresource
	gDevice->CreateBuffer(&cameraDesc, &cameraData, &gCameraMatrixBuffer);

	/////////////////
	// Ambient	   //
	///////////////// 

	//allocate space in memory aligned to a multitude of 16
	gAmbientSpecularData = (AmbientSpecular*)_aligned_malloc(sizeof(AmbientSpecular), 16);

	//create a description objekt defining how the buffer should be handled
	D3D11_BUFFER_DESC ambientDesc;
	ZeroMemory(&ambientDesc, sizeof(ambientDesc));
	ambientDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ambientDesc.ByteWidth = sizeof(AmbientSpecular);
	ambientDesc.Usage = D3D11_USAGE_DYNAMIC;
	ambientDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//bind light data to a subresource
	D3D11_SUBRESOURCE_DATA ambientData;
	ZeroMemory(&ambientData, sizeof(ambientData));
	ambientData.pSysMem = gAmbientSpecularData;
	ambientData.SysMemPitch = 0;
	ambientData.SysMemSlicePitch = 0;

	//create buffer for light subresource
	gDevice->CreateBuffer(&ambientDesc, &ambientData, &gAmbientSpecularBuffer);

	/////////////////
	// Light	   //
	///////////////// 

	//create a description objekt defining how the buffer should be handled
	D3D11_BUFFER_DESC lightDesc;
	ZeroMemory(&lightDesc, sizeof(lightDesc));
	lightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightDesc.ByteWidth = sizeof(LightData);
	lightDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//create buffer for light subresource
	gDevice->CreateBuffer(&lightDesc, nullptr, &gLightDataBuffer);

	//////////////////////////////////////
	// World/View/Projection For Lights //
	//////////////////////////////////////

	//allocate space in memory aligned to a multitude of 16
	gLightMatrix = (LightWVP*)_aligned_malloc(sizeof(LightWVP), 16);

	// Describe the Constant Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(LightWVP);
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//bind matrix data to a subresource
	D3D11_SUBRESOURCE_DATA lightData;
	lightData.pSysMem = gLightMatrix;
	lightData.SysMemPitch = 0;
	lightData.SysMemSlicePitch = 0;

	//create buffer for our world,view and projection matrix
	gDevice->CreateBuffer(&bufferDesc, &lightData, &gLightMatrixBuffer);

	///////////////////////////////////////
	// World/View/Projection For Objects //
	///////////////////////////////////////

	//allocate space in memory aligned to a multitude oAf 16
	gWorldMatrix = (CameraWVP*)_aligned_malloc(sizeof(CameraWVP), 16);

	// Describe the Constant Buffer
	D3D11_BUFFER_DESC worldDesc;
	memset(&worldDesc, 0, sizeof(worldDesc));
	worldDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	worldDesc.Usage = D3D11_USAGE_DYNAMIC;
	worldDesc.ByteWidth = sizeof(CameraWVP);
	worldDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//bind matrix data to a subresource
	D3D11_SUBRESOURCE_DATA worldData;
	worldData.pSysMem = gWorldMatrix;
	worldData.SysMemPitch = 0;
	worldData.SysMemSlicePitch = 0;

	//create buffer for our world,view and projection matrix
	gDevice->CreateBuffer(&worldDesc, &worldData, &gWorldMatrixBuffer);
}

void setWorldSpace() {
	//alternativly use XMMatrixRotationX(rotation);
	/*XMMATRIX WorldX = XMMatrixSet(
	1.0, 0.0, 0.0, 0.0,
	0.0, (float)cos(rotation), (float)sin(rotation), 0.0,
	0.0, (float)-sin(rotation), (float)cos(rotation), 0.0,
	0.0, 0.0, 0.0, 1.0
	);*/

	//alternativly use XMMatrixRotationY(rotation);
	XMMATRIX WorldY = XMMatrixSet(
		(float)cos(rotation), 0.0, (float)-sin(rotation), 0.0,
		0.0, 1.0, 0.0, 0.0,
		(float)sin(rotation), 0.0, (float)cos(rotation), 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	//alternativly use XMMatrixRotationZ(rotation); 
	/*XMMATRIX WorldZ = XMMatrixSet(
	(float)cos(rotation), (float)sin(rotation), 0.0, 0.0,
	(float)-sin(rotation), (float)cos(rotation), 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
	);*/

	World = WorldY;
	//World = XMMatrixMultiply(WorldX, World);
	//World = XMMatrixMultiply(World, WorldZ);	
}

void setCameraViewProjectionSpace() {
	View = XMMatrixLookAtLH(
		camera.GetCamPos(),
		camera.GetCamTarget(),
		camera.GetCamUp()
	);
	View = XMMatrixTranspose(View);

	Projection = XMMatrixPerspectiveFovLH(
		(float)XM_PI*FOV,
		(float)W_WIDTH / (float)W_HEIGHT,
		PROJECTION_NEAR_Z,
		PROJECTION_FAR_Z
	);
	Projection = XMMatrixTranspose(Projection);
}

void setLightViewProjectionSpace(LightSource* light) {
	View = light->getView();

	Projection = light->getProjection();
}

void updateCameraWorldViewProjection() {
	gWorldMatrix->World = World;
	gWorldMatrix->ViewProjection = XMMatrixMultiply(Projection, View);

	//create a subresource to hold our data while we copy between cpu and gpu memory
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	//copy and map our cpu memory to our gpu buffert
	gDeviceContext->Map(gWorldMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, gWorldMatrix, sizeof(CameraWVP));
	gDeviceContext->Unmap(gWorldMatrixBuffer, 0);
}

void updateLightWorldViewProjection() {
	gLightMatrix->WorldViewProjection = XMMatrixMultiply(Projection, XMMatrixMultiply(View, World));

	//create a subresource to hold our data while we copy between cpu and gpu memory
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	//copy and map our cpu memory to our gpu buffert
	gDeviceContext->Map(gWorldMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, gLightMatrix, sizeof(LightWVP));
	gDeviceContext->Unmap(gWorldMatrixBuffer, 0);
}

void updateCameraValues() {
	//Walking in Memphis
	Vector4 CameraPos = camera.GetCamPos();
	
	//temmp static camera
	gCameraMatrix->Origin = camera.GetCamPos();
	gCameraMatrix->Focus = camera.GetCamTarget();

	//create a subresource to hold our data while we copy between cpu and gpu memory
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	//copy and map our cpu memory to our gpu buffert
	gDeviceContext->Map(gCameraMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, gCameraMatrix, sizeof(CameraMatrix));
	gDeviceContext->Unmap(gCameraMatrixBuffer, 0);
};

void setSpecularValues(XMVECTOR specular) {
	gAmbientSpecularData->Specular = specular;

	//create a subresource to hold our data while we copy between cpu and gpu memory
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	//copy and map our cpu memory to our gpu buffert
	gDeviceContext->Map(gAmbientSpecularBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, gAmbientSpecularData, sizeof(AmbientSpecular));
	gDeviceContext->Unmap(gAmbientSpecularBuffer, 0);
};