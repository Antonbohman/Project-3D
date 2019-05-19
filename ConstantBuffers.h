#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "Global.h"

using namespace DirectX;

void CreateConstantBuffer();

void setWorldSpace(WorldSpace world);

void setCameraViewProjectionSpace();

void updateCameraWorldViewProjection();

void updateObjectWorldSpace();

void updateLightViewProjection(LightSource* light);

void updateCameraValues();

void updateSpecularValues(XMVECTOR ambient, XMVECTOR diffuse, XMVECTOR specular);

void updateRenderingOptions();