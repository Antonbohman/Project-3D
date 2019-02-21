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

HRESULT CreateDeferredVS();
HRESULT CreateDeferredGS();
HRESULT CreateDeferredPS();
HRESULT CreateDeferredCS();
HRESULT CreateLightVS();
HRESULT CreateLightPS();

void SetDeferredShaders();
void SetLightShaders();
void setVertexBuffer(ID3D11Buffer* buffer, UINT32 size, UINT32 offset);