#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>


//TOOL KIT
//#include "CommonStates.h"
#include "DDSTextureLoader.h"
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
#include "Global.h"

using namespace DirectX;
using namespace SimpleMath;

void createVertexBuffer(int nrOfVertices, TriangleVertex ArrOfVert[]);

void loadHeightMap(char* filename);

void loadMultiTextureMap(char* filename);

void LoadObjectFile(char* filename, XMINT3 offset);

void loadTexture(const char* filePath);