#include "ConstantBuffers.h"

void CreateConstantBuffer() {
	///////////////////////
	// Rendering Options //
	/////////////////////// 

	//allocate space in memory aligned to a multitude of 16
	gRenderingOptionsData = (RenderOptions*)_aligned_malloc(sizeof(RenderOptions), 16);

	//create a description objekt defining how the buffer should be handled
	D3D11_BUFFER_DESC optDesc;
	ZeroMemory(&optDesc, sizeof(optDesc));
	optDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	optDesc.ByteWidth = sizeof(RenderOptions);
	optDesc.Usage = D3D11_USAGE_DYNAMIC;
	optDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//bind light data to a subresource
	D3D11_SUBRESOURCE_DATA optData;
	ZeroMemory(&optData, sizeof(optData));
	optData.pSysMem = gRenderingOptionsData;
	optData.SysMemPitch = 0;
	optData.SysMemSlicePitch = 0;

	//create buffer for light subresource
	gDevice->CreateBuffer(&optDesc, &optData, &gRenderingOptionsBuffer);


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

void setWorldSpace(WorldSpace world) {
	XMMATRIX rotationX = XMMatrixRotationX(world.rotation_x);
	XMMATRIX rotationY = XMMatrixRotationY(world.rotation_y);
	XMMATRIX rotationZ = XMMatrixRotationZ(world.rotation_z);

	XMMATRIX offset = XMMatrixTranslation(world.offset_x, world.offset_y, world.offset_z);

	XMMATRIX scale = XMMatrixScaling(world.scale_x, world.scale_y, world.scale_z);
	
	//XMMATRIX rotate = XMMatrixMultiply(rotationX, XMMatrixMultiply(rotationY, rotationZ));

	XMMATRIX rotate = XMMatrixMultiply(rotationX, XMMatrixMultiply(rotationY, rotationZ));

	World = XMMatrixMultiply(scale, rotate);
	World = XMMatrixMultiply(World, offset);
}

void setCameraViewProjectionSpace() {
	View = XMMatrixLookAtLH(
		camera.GetCamPos(),
		camera.GetCamTarget(),
		camera.GetCamUp()
	);
	View = XMMatrixTranspose(View);

	if (renderOpt & RENDER_MULTI_VIEWPORT) {
		ViewRotated[0] = XMMatrixLookAtLH(
			camera.GetCamPos(),
			camera.GetCamPos() + camera.GetCamRight(), 
			camera.GetCamUp()
		);
		
		ViewRotated[1] = XMMatrixLookAtLH(
			camera.GetCamPos(),
			camera.GetCamPos() - camera.GetCameraNormal(),
			camera.GetCamUp()
		);

		ViewRotated[2] = XMMatrixLookAtLH(
			camera.GetCamPos(),
			camera.GetCamPos() - camera.GetCamRight(),
			camera.GetCamUp()
		);

		for (int i = 0; i < 3; i++) {
			ViewRotated[i] = XMMatrixTranspose(ViewRotated[i]);
		}
	}

	Projection = XMMatrixPerspectiveFovLH(
		(float)XM_PI*FOV,
		(float)W_WIDTH / (float)W_HEIGHT,
		PROJECTION_NEAR_Z,
		PROJECTION_FAR_Z
	);
	Projection = XMMatrixTranspose(Projection);
}

void updateCameraWorldViewProjection() {
	gWorldMatrix->World = World;
	gWorldMatrix->ViewProjection = XMMatrixMultiply(Projection, View);

	if (renderOpt & RENDER_MULTI_VIEWPORT) {
		gWorldMatrix->RotatedViewProjection[0] = XMMatrixMultiply(Projection, ViewRotated[0]);
		gWorldMatrix->RotatedViewProjection[1] = XMMatrixMultiply(Projection, ViewRotated[1]);
		gWorldMatrix->RotatedViewProjection[2] = XMMatrixMultiply(Projection, ViewRotated[2]);
	}

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
	gLightMatrix->ViewProjection = light->getViewProjection(0);
	gLightMatrix->RotatedViewProjection[0] = light->getViewProjection(1);
	gLightMatrix->RotatedViewProjection[1] = light->getViewProjection(2);
	gLightMatrix->RotatedViewProjection[2] = light->getViewProjection(3);
	gLightMatrix->RotatedViewProjection[3] = light->getViewProjection(4);
	gLightMatrix->RotatedViewProjection[4] = light->getViewProjection(5);

	//create a subresource to hold our data while we copy between cpu and gpu memory
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	//copy and map our cpu memory to our gpu buffert
	gDeviceContext->Map(gLightMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, gLightMatrix, sizeof(LightVP));
	gDeviceContext->Unmap(gLightMatrixBuffer, 0);
}

void updateCameraValues() {
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

void updateSpecularValues(XMVECTOR ambient, XMVECTOR diffuse, XMVECTOR specular) {
	gAmbientSpecularData->Ambient = ambient;
	gAmbientSpecularData->Diffuse = diffuse;
	gAmbientSpecularData->Specular = specular;

	//create a subresource to hold our data while we copy between cpu and gpu memory
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	//copy and map our cpu memory to our gpu buffert
	gDeviceContext->Map(gAmbientSpecularBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, gAmbientSpecularData, sizeof(AmbientSpecular));
	gDeviceContext->Unmap(gAmbientSpecularBuffer, 0);
};

void updateRenderingOptions() {
	gRenderingOptionsData->splitView = renderOpt & RENDER_MULTI_VIEWPORT ? true : false;
	gRenderingOptionsData->renderMode = renderMode;

	//create a subresource to hold our data while we copy between cpu and gpu memory
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	//copy and map our cpu memory to our gpu buffert
	gDeviceContext->Map(gRenderingOptionsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, gRenderingOptionsData, sizeof(RenderOptions));
	gDeviceContext->Unmap(gRenderingOptionsBuffer, 0);
};