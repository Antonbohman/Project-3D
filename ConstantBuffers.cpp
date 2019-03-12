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


	/////////////////////////////
	// World Space For Objects //
	/////////////////////////////

	//allocate space in memory aligned to a multitude of 16
	gObjectMatrix = (ObjectW*)_aligned_malloc(sizeof(ObjectW), 16);

	// Describe the Constant Buffer
	D3D11_BUFFER_DESC objectDesc;
	memset(&objectDesc, 0, sizeof(objectDesc));
	objectDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	objectDesc.Usage = D3D11_USAGE_DYNAMIC;
	objectDesc.ByteWidth = sizeof(ObjectW);
	objectDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//bind matrix data to a subresource
	D3D11_SUBRESOURCE_DATA objectData;
	objectData.pSysMem = gObjectMatrix;
	objectData.SysMemPitch = 0;
	objectData.SysMemSlicePitch = 0;

	//create buffer for our world,view and projection matrix
	gDevice->CreateBuffer(&objectDesc, &objectData, &gObjectMatrixBuffer);

	////////////////////////////////
	// View/Projection For Lights //
	////////////////////////////////

	//allocate space in memory aligned to a multitude of 16
	gLightMatrix = (LightVP*)_aligned_malloc(sizeof(LightVP), 16);

	// Describe the Constant Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(LightVP);
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

	//allocate space in memory aligned to a multitude of 16
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

void setWorldSpace(WorldSpace world) {
	XMMATRIX rotationX = XMMatrixRotationX(world.rotation_x);
	XMMATRIX rotationY = XMMatrixRotationX(world.rotation_y);
	XMMATRIX rotationZ = XMMatrixRotationX(world.rotation_z);

	XMMATRIX offset = XMMatrixTranslation(world.offset_x, world.offset_y, world.offset_z);

	XMMATRIX scale = XMMatrixScaling(world.scale_x, world.scale_y, world.scale_z);

	World = XMMatrixMultiply(rotationX, XMMatrixMultiply(rotationY, rotationZ));
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
	View = light->getView(0);

	if (light->getLightType() == L_POINT) {
		ViewRotated[0] = light->getView(1);
		ViewRotated[1] = light->getView(2);
		ViewRotated[2] = light->getView(3);
		ViewRotated[3] = light->getView(4);
		ViewRotated[4] = light->getView(5);
	}

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

void updateObjectWorldSpace() {
	gObjectMatrix->World = World;

	//create a subresource to hold our data while we copy between cpu and gpu memory
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	//copy and map our cpu memory to our gpu buffert
	gDeviceContext->Map(gObjectMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, gObjectMatrix, sizeof(ObjectW));
	gDeviceContext->Unmap(gObjectMatrixBuffer, 0);
}

void updateLightViewProjection(LightSource* light) {
	gLightMatrix->LightType = light->getLightType();
	XMStoreFloat3(&gLightMatrix->Origin, light->getOrigin());
	gLightMatrix->ViewProjection = XMMatrixMultiply(Projection, View);
	
	if (gLightMatrix->LightType == L_POINT) {
		gLightMatrix->RotatedViewProjection[0] = XMMatrixMultiply(Projection, ViewRotated[0]);
		gLightMatrix->RotatedViewProjection[1] = XMMatrixMultiply(Projection, ViewRotated[1]);
		gLightMatrix->RotatedViewProjection[2] = XMMatrixMultiply(Projection, ViewRotated[2]);
		gLightMatrix->RotatedViewProjection[3] = XMMatrixMultiply(Projection, ViewRotated[3]);
		gLightMatrix->RotatedViewProjection[4] = XMMatrixMultiply(Projection, ViewRotated[4]);
	}

	//create a subresource to hold our data while we copy between cpu and gpu memory
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	//copy and map our cpu memory to our gpu buffert
	gDeviceContext->Map(gLightMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, gLightMatrix, sizeof(LightVP));
	gDeviceContext->Unmap(gLightMatrixBuffer, 0);
}

void updateCameraValues() {
	//Walking in Memphis
	Vector4 CameraPos = camera.GetCamPos();
	XMINT2 roundedPos;
	roundedPos.x = CameraPos.x;
	roundedPos.y = CameraPos.y;

	if (float(roundedPos.x) + 0.5f > CameraPos.x) roundedPos.x++;
	if (float(roundedPos.y) + 0.5f > CameraPos.y) roundedPos.y++;

	int index = (g_heightmap.imageWidth * (roundedPos.y + (g_heightmap.imageHeight / 2)) +
		(roundedPos.x + (g_heightmap.imageWidth / 2)));

	float newHeight = (g_heightmap.verticesPos[index].y + 1.5f);


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