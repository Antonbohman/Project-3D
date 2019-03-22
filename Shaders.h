#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "Global.h"

using namespace DirectX;

HRESULT CreateShaders();
void DestroyShaders();

HRESULT CreateWireframePS();
HRESULT CreateShadowVS();
HRESULT CreateShadowGS();
HRESULT CreateShadowPS();
HRESULT CreateDeferredVS();
HRESULT CreateDeferredGS();
HRESULT CreateDeferredPS();
HRESULT CreateDeferredMTPS();
HRESULT CreateDeferredCS();
HRESULT CreateLightVS();
HRESULT CreateLightPS();

void setWireframeShaders();
void setShadowShaders();
void SetDeferredShaders();
void SetComputeShaders();
void SetBlendShaders();
void SetLightShaders();
void setVertexBuffer(ID3D11Buffer* buffer, UINT32 size, UINT32 offset);