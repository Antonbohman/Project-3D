#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "Global.h"

using namespace DirectX;

void CreateConstantBuffer();

void setWorldSpace();

void setCameraViewProjectionSpace();

void setLightViewProjectionSpace(LightSource* light);

void updateCameraWorldViewProjection();

void updateLightWorldViewProjection(); 

void updateCameraValues();

void setSpecularValues(XMVECTOR specular);