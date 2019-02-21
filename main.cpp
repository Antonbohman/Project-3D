#pragma once
#include <windows.h>
#include <chrono>
#include <algorithm>
#include <Mouse.h>
#include <Keyboard.h>

std::unique_ptr<DirectX::Mouse>m_mouse;
std::unique_ptr<DirectX::Keyboard>m_keyboard;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

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
//#include "SpriteBatch.h"
//#include "SpriteFont.h"
//#include "VertexTypes.h"
//#include "WICTextureLoader.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "KeyInput.h"
#include "Camera.h"
#include "LightSource.h"
#include "Global.h"
#include "Shaders.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace std::chrono;

Camera camera({ 0.0f,10.0f,-20.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f });

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HRESULT CreateDirect3DContext(HWND wndHandle);

//Camera camera({ 0.0f,10.0f,-20.0f,0.0f }, { 0.0f,0.0f,0.0f,0.0f });


void CreateDeferredQuad() {
	PositionVertex triangleVertices[6] =
	{
		-1, -1,	//v0 pos
		-1, 1,	//v1 pos
		1, -1,    //v2 pos

		//----//

		1, 1, 	//v3 pos
		1, -1,	//v4 pos
		-1, 1,    //v5 pos
	};

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	// what type of buffer will this be?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = sizeof(PositionVertex) * 6;

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;

	// create a Vertex Buffer
	gDevice->CreateBuffer(&bufferDesc, &data, &gDeferredQuadBuffer);
}

void CreateHeightmapData(Heightmap heightmap) {
	// Array of Structs (AoS)
	//63 900 vertices 300x213
	//384404

	//gHeightfactor = 25.5 * 6;

	XMFLOAT4 bedrock =
	{
		35.0f / 255,
		98.0f / 255,
		175.0f / 255,
		0.0f
	};
	XMFLOAT4 grass =
	{
		53.0f / 255,
		178.0f / 255,
		0.0f / 255,
		(2.55f / gHeightfactor)
	};
	XMFLOAT4 mountain =
	{
		128.0f / 255,
		128.0f / 255,
		128.0f / 255,
		(30.0f / gHeightfactor)
	};
	XMFLOAT4 snow =
	{
		180.0f / 255,
		180.0f / 255,
		180.0f / 255,
		(200.0f / gHeightfactor)
	};
	gnrOfVertices = 4 + ((heightmap.imageWidth - 2) * 4 * 2) + ((heightmap.imageHeight - 2) * 4 * 2) + ((heightmap.imageHeight - 1) * (heightmap.imageWidth - 1) * 6);

	//TriangleVertex* gMap = new TriangleVertex[gnrOfVertices];
	//TriangleVertex gMap[3820];

	gMap = new TriangleVertex[gnrOfVertices];

	int vertNr = 0;

	for (int i = 0; i < heightmap.imageHeight - 1; i++)
	{
		int X = (i * heightmap.imageWidth);
		int Y = ((i + 1) * heightmap.imageWidth);

		for (int k = 0/*i * heightmap.imageWidth*/; k < /*(i + 1) **/ heightmap.imageWidth - 1; k++)
		{
			/*Position*/
			gMap[vertNr].x = heightmap.verticesPos[Y].x - (heightmap.imageWidth / 2);
			gMap[vertNr].y = heightmap.verticesPos[Y].y;
			gMap[vertNr].z = heightmap.verticesPos[Y].z - (heightmap.imageHeight / 2);
			/*Colour*/
			if (gMap[vertNr].y > grass.w)
			{
				if (gMap[vertNr].y > mountain.w)
				{
					if (gMap[vertNr].y > snow.w)
					{
						gMap[vertNr].r = snow.x;
						gMap[vertNr].g = snow.y;
						gMap[vertNr].b = snow.z;
					}
					else
					{
						gMap[vertNr].r = mountain.x;
						gMap[vertNr].g = mountain.y;
						gMap[vertNr].b = mountain.z;
					}
				}
				else
				{
					gMap[vertNr].r = grass.x;
					gMap[vertNr].g = grass.y;
					gMap[vertNr].b = grass.z;
				}
			}
			else
			{
				gMap[vertNr].r = bedrock.x;
				gMap[vertNr].g = bedrock.y;
				gMap[vertNr].b = bedrock.z;
			}
			//gMap[vertNr].r = heightmap.verticesPos[Y].y;
			//gMap[vertNr].g = heightmap.verticesPos[Y].y;
			//gMap[vertNr].b = heightmap.verticesPos[Y].y;

			/*UV*/
			gMap[vertNr].u = 0.0f;
			gMap[vertNr].v = 1.0f;

			vertNr++;

			/*..............................*/

			/*Position*/
			gMap[vertNr].x = heightmap.verticesPos[X + 1].x - (heightmap.imageWidth / 2);
			gMap[vertNr].y = heightmap.verticesPos[X + 1].y;
			gMap[vertNr].z = heightmap.verticesPos[X + 1].z - (heightmap.imageHeight / 2);
			/*Colour*/
			if (gMap[vertNr].y > grass.w)
			{
				if (gMap[vertNr].y > mountain.w)
				{
					if (gMap[vertNr].y > snow.w)
					{
						gMap[vertNr].r = snow.x;
						gMap[vertNr].g = snow.y;
						gMap[vertNr].b = snow.z;
					}
					else
					{
						gMap[vertNr].r = mountain.x;
						gMap[vertNr].g = mountain.y;
						gMap[vertNr].b = mountain.z;
					}
				}
				else
				{
					gMap[vertNr].r = grass.x;
					gMap[vertNr].g = grass.y;
					gMap[vertNr].b = grass.z;
				}
			}
			else
			{
				gMap[vertNr].r = bedrock.x;
				gMap[vertNr].g = bedrock.y;
				gMap[vertNr].b = bedrock.z;
			}
			//gMap[vertNr].r = heightmap.verticesPos[Y].y;
			//gMap[vertNr].g = heightmap.verticesPos[Y].y;
			//gMap[vertNr].b = heightmap.verticesPos[Y].y;

			/*UV*/
			gMap[vertNr].u = 1.0f;
			gMap[vertNr].v = 1.0f;

			vertNr++;

			/*-------------------------------*/

			/*Position*/
			gMap[vertNr].x = heightmap.verticesPos[X].x - (heightmap.imageWidth / 2);
			gMap[vertNr].y = heightmap.verticesPos[X].y;
			gMap[vertNr].z = heightmap.verticesPos[X].z - (heightmap.imageHeight / 2);
			/*Colour*/
			if (gMap[vertNr].y > grass.w)
			{
				if (gMap[vertNr].y > mountain.w)
				{
					if (gMap[vertNr].y > snow.w)
					{
						gMap[vertNr].r = snow.x;
						gMap[vertNr].g = snow.y;
						gMap[vertNr].b = snow.z;
					}
					else
					{
						gMap[vertNr].r = mountain.x;
						gMap[vertNr].g = mountain.y;
						gMap[vertNr].b = mountain.z;
					}
				}
				else
				{
					gMap[vertNr].r = grass.x;
					gMap[vertNr].g = grass.y;
					gMap[vertNr].b = grass.z;
				}
			}
			else
			{
				gMap[vertNr].r = bedrock.x;
				gMap[vertNr].g = bedrock.y;
				gMap[vertNr].b = bedrock.z;
			}
			//gMap[vertNr].r = heightmap.verticesPos[Y].y;
			//gMap[vertNr].g = heightmap.verticesPos[Y].y;
			//gMap[vertNr].b = heightmap.verticesPos[Y].y;

			/*UV*/
			gMap[vertNr].u = 0.0f;
			gMap[vertNr].v = 0.0f;


			vertNr++;
			X++;

			/*-------------------------------*/
			/*Next triangle*/

			/*Position*/
			gMap[vertNr].x = heightmap.verticesPos[Y].x - (heightmap.imageWidth / 2);
			gMap[vertNr].y = heightmap.verticesPos[Y].y;
			gMap[vertNr].z = heightmap.verticesPos[Y].z - (heightmap.imageHeight / 2);
			/*Colour*/
			if (gMap[vertNr].y > grass.w)
			{
				if (gMap[vertNr].y > mountain.w)
				{
					if (gMap[vertNr].y > snow.w)
					{
						gMap[vertNr].r = snow.x;
						gMap[vertNr].g = snow.y;
						gMap[vertNr].b = snow.z;
					}
					else
					{
						gMap[vertNr].r = mountain.x;
						gMap[vertNr].g = mountain.y;
						gMap[vertNr].b = mountain.z;
					}
				}
				else
				{
					gMap[vertNr].r = grass.x;
					gMap[vertNr].g = grass.y;
					gMap[vertNr].b = grass.z;
				}
			}
			else
			{
				gMap[vertNr].r = bedrock.x;
				gMap[vertNr].g = bedrock.y;
				gMap[vertNr].b = bedrock.z;
			}
			//gMap[vertNr].r = heightmap.verticesPos[Y].y;
			//gMap[vertNr].g = heightmap.verticesPos[Y].y;
			//gMap[vertNr].b = heightmap.verticesPos[Y].y;

			/*UV*/
			gMap[vertNr].u = 1.0f;
			gMap[vertNr].v = 1.0f;

			vertNr++;

			/*..............................*/

			/*Position*/
			gMap[vertNr].x = heightmap.verticesPos[X + heightmap.imageWidth].x - (heightmap.imageWidth / 2);
			gMap[vertNr].y = heightmap.verticesPos[X + heightmap.imageWidth].y;
			gMap[vertNr].z = heightmap.verticesPos[X + heightmap.imageWidth].z - (heightmap.imageHeight / 2);
			/*Colour*/
			if (gMap[vertNr].y > grass.w)
			{
				if (gMap[vertNr].y > mountain.w)
				{
					if (gMap[vertNr].y > snow.w)
					{
						gMap[vertNr].r = snow.x;
						gMap[vertNr].g = snow.y;
						gMap[vertNr].b = snow.z;
					}
					else
					{
						gMap[vertNr].r = mountain.x;
						gMap[vertNr].g = mountain.y;
						gMap[vertNr].b = mountain.z;
					}
				}
				else
				{
					gMap[vertNr].r = grass.x;
					gMap[vertNr].g = grass.y;
					gMap[vertNr].b = grass.z;
				}
			}
			else
			{
				gMap[vertNr].r = bedrock.x;
				gMap[vertNr].g = bedrock.y;
				gMap[vertNr].b = bedrock.z;
			}
			//gMap[vertNr].r = heightmap.verticesPos[Y].y;
			//gMap[vertNr].g = heightmap.verticesPos[Y].y;
			//gMap[vertNr].b = heightmap.verticesPos[Y].y;

			/*UV*/
			gMap[vertNr].u = 1.0f;
			gMap[vertNr].v = 0.0f;

			vertNr++;

			/*-------------------------------*/

			/*Position*/
			gMap[vertNr].x = heightmap.verticesPos[X].x - (heightmap.imageWidth / 2);
			gMap[vertNr].y = heightmap.verticesPos[X].y;
			gMap[vertNr].z = heightmap.verticesPos[X].z - (heightmap.imageHeight / 2);
			/*Colour*/
			if (gMap[vertNr].y > grass.w)
			{
				if (gMap[vertNr].y > mountain.w)
				{
					if (gMap[vertNr].y > snow.w)
					{
						gMap[vertNr].r = snow.x;
						gMap[vertNr].g = snow.y;
						gMap[vertNr].b = snow.z;
					}
					else
					{
						gMap[vertNr].r = mountain.x;
						gMap[vertNr].g = mountain.y;
						gMap[vertNr].b = mountain.z;
					}
				}
				else
				{
					gMap[vertNr].r = grass.x;
					gMap[vertNr].g = grass.y;
					gMap[vertNr].b = grass.z;
				}
			}
			else
			{
				gMap[vertNr].r = bedrock.x;
				gMap[vertNr].g = bedrock.y;
				gMap[vertNr].b = bedrock.z;
			}
			//gMap[vertNr].r = heightmap.verticesPos[Y].y;
			//gMap[vertNr].g = heightmap.verticesPos[Y].y;
			//gMap[vertNr].b = heightmap.verticesPos[Y].y;

			/*UV*/
			gMap[vertNr].u = 0.0f;
			gMap[vertNr].v = 0.0f;

			vertNr++;
			Y++;
		}
	}

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	// what type of buffer will this be?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = sizeof(TriangleVertex) * gnrOfVertices;

	//int size = sizeof(gMap);

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)gMap;

	// create a Vertex Buffer
	HRESULT error;
	error = gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBufferMap);
	//delete gMap;
	//4ret
}

void CreateConstantBuffer() {
	/////////////////
	// Camera	   //
	///////////////// 

	//allocate space in memory aligned to a multitude of 16
	gCameraMatrix = (CameraMatrix*)_aligned_malloc(sizeof(CameraMatrix), 16);

	//temmp static camera
	gCameraMatrix->Origin = XMVectorSet(0.0f, 10.0f, -200.0f, 0.0f);
	gCameraMatrix->Focus = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

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

	//since we won't be updating these values while program is running
	gAmbientSpecularData->Ambient = XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f);
	gAmbientSpecularData->Specular = XMVectorSet(0.0f, 0.0f, 0.0f, 10000.0f);

	//create a description objekt defining how the buffer should be handled
	D3D11_BUFFER_DESC ambientDesc;
	ZeroMemory(&ambientDesc, sizeof(ambientDesc));
	ambientDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ambientDesc.ByteWidth = sizeof(XMVECTOR);
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

	///////////////////////////
	// World/View/Projection //
	///////////////////////////

	//allocate space in memory aligned to a multitude of 16
	gWorldMatrix = (WorldMatrix*)_aligned_malloc(sizeof(WorldMatrix), 16);

	// Describe the Constant Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(WorldMatrix);
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//bind matrix data to a subresource
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = gWorldMatrix;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	//create buffer for our world,view and projection matrix
	gDevice->CreateBuffer(&bufferDesc, &data, &gWorldMatrixBuffer);
}

HRESULT CreateSampling() {
	D3D11_SAMPLER_DESC samplingDesc;
	// Create a texture sampler state description.
	samplingDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplingDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplingDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplingDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplingDesc.MipLODBias = 0.0f;
	samplingDesc.MaxAnisotropy = 1;
	samplingDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplingDesc.BorderColor[0] = 0;
	samplingDesc.BorderColor[1] = 0;
	samplingDesc.BorderColor[2] = 0;
	samplingDesc.BorderColor[3] = 0;
	samplingDesc.MinLOD = 0;
	samplingDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	return gDevice->CreateSamplerState(&samplingDesc, &gSampling);
}

void CreateLigths() {
	nrOfLights = 1;
	Lights = new LightSource[nrOfLights];

	Lights[0] = LightSource(0, XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f);
}

void SetViewport() {
	D3D11_VIEWPORT vp;
	vp.Width = (float)W_WIDTH;
	vp.Height = (float)W_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gDeviceContext->RSSetViewports(1, &vp);
}

void LoadObjectFile(char* filename)
{
	FILE* fileptr;
	fileptr = fopen(filename, "r");
	if (fileptr == NULL)
	{
		return;
	}
	int loopControl = 1;
	char line[128];

	XMFLOAT3* arrOfVertices = new XMFLOAT3[100];
	int arrSize = 100;
	int nrOfVert = 0;

	bool textureCordStart = false;
	XMFLOAT2* arrOfTxtCord = nullptr;
	int nrOfTxtCord = 0;
	int txtCoordArrSize = 0;

	bool normalStart = false;
	XMFLOAT3* arrOfNormals = nullptr;
	int nrOfNormals = 0;
	int normArrSize = 0;

	bool indexStart = false;
	XMINT3* arrOfIndex = nullptr;
	gnrOfFaces = 0;
	int objArrSize = 0;

	while (loopControl != EOF)
	{
		loopControl = fscanf(fileptr, "%s", line);
		if (loopControl == EOF) {}
		else
		{
			if (strcmp(line, "v") == 0)
			{
				XMFLOAT3 vertex;
				fscanf(fileptr, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				if (nrOfVert == arrSize)
				{
					XMFLOAT3* tempArr = new XMFLOAT3[arrSize + 50];
					for (int i = 0; i < arrSize; i++)
					{
						tempArr[i] = arrOfVertices[i];
					}
					delete arrOfVertices;
					arrOfVertices = tempArr;

					arrSize += 50;
				}
				arrOfVertices[nrOfVert] = vertex;
				nrOfVert++;
			}
			else if (strcmp(line, "vt") == 0)
			{
				if (textureCordStart == false)
				{
					txtCoordArrSize = nrOfVert;
					arrOfTxtCord = new XMFLOAT2[txtCoordArrSize];
					textureCordStart = true;
				}
				if (nrOfTxtCord == txtCoordArrSize)
				{
					XMFLOAT2* tempArr = new XMFLOAT2[txtCoordArrSize + 50];
					for (int i = 0; i < txtCoordArrSize; i++)
					{
						tempArr[i] = arrOfTxtCord[i];
					}
					delete arrOfTxtCord;
					arrOfTxtCord = tempArr;

					txtCoordArrSize += 50;
				}
				XMFLOAT2 vertex;
				fscanf(fileptr, "%f %f\n", &vertex.x, &vertex.y);

				arrOfTxtCord[nrOfTxtCord] = vertex;
				nrOfTxtCord++;
			}
			else if (strcmp(line, "vn") == 0)
			{
				if (normalStart == false)
				{
					normArrSize = nrOfVert;
					arrOfNormals = new XMFLOAT3[normArrSize];
					normalStart = true;
				}
				if (nrOfNormals == normArrSize)
				{
					XMFLOAT3* tempArr = new XMFLOAT3[normArrSize + 50];
					for (int i = 0; i < normArrSize; i++)
					{
						tempArr[i] = arrOfNormals[i];
					}
					delete arrOfNormals;
					arrOfNormals = tempArr;

					normArrSize += 50;
				}

				XMFLOAT3 vertex;
				fscanf(fileptr, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

				arrOfNormals[nrOfNormals] = vertex;
				nrOfNormals++;
			}
			else if (strcmp(line, "f") == 0)
			{
				XMINT3 vertex[3];
				fscanf(fileptr, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex[0].x, &vertex[0].y, &vertex[0].z, &vertex[1].x, &vertex[1].y, &vertex[1].z, &vertex[2].x, &vertex[2].y, &vertex[2].z);
				/*x innehåller vertex positioner, y innehåller texture, z innehåller normaler*/
				if (indexStart == false)
				{
					objArrSize = (nrOfVert * 1.5);
					gObject = new TriangleVertex[objArrSize];
					indexStart = true;
				}
				if (gnrOfFaces + 3 >= objArrSize)
				{
					TriangleVertex* tempArr = new TriangleVertex[objArrSize + 50];
					for (int i = 0; i < gnrOfFaces; i++)
					{
						tempArr[i] = gObject[i];
					}
					delete gObject;
					gObject = tempArr;

					objArrSize += 50;
				}
				for (int i = 0; i < 3; i++)
				{
					gObject[gnrOfFaces + i].x = arrOfVertices[vertex[i].x - 1].x;
					gObject[gnrOfFaces + i].y = arrOfVertices[vertex[i].x - 1].y;
					gObject[gnrOfFaces + i].z = arrOfVertices[vertex[i].x - 1].z;

					//gObject[gnrOfFaces + i].x_n = arrOfNormals[vertex[i].y - 1].x;
					//gObject[gnrOfFaces + i].y_n = arrOfNormals[vertex[i].y - 1].y;
					//gObject[gnrOfFaces + i].z_n = arrOfNormals[vertex[i].y - 1].z;

					gObject[gnrOfFaces + i].u = arrOfTxtCord[vertex[i].y - 1].x;
					gObject[gnrOfFaces + i].v = arrOfTxtCord[vertex[i].y - 1].y;

					gObject[gnrOfFaces + i].r = 0.5f;
					gObject[gnrOfFaces + i].g = 0.0f;
					gObject[gnrOfFaces + i].b = 0.5f;
				}
				gnrOfFaces += 3;
			}
		}
	}
	fclose(fileptr);

	TriangleVertex* objectArray = new TriangleVertex[gnrOfFaces];
	for (int i = 0; i < gnrOfFaces; i++)
	{
		objectArray[i] = gObject[i];
	}

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	// what type of buffer will this be?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = sizeof(TriangleVertex) * gnrOfFaces;

	//int size = sizeof(gMap);

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)objectArray;

	// create a Vertex Buffer
	HRESULT error;
	error = gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBufferObj);
	int ok = 0;

	//delete[] objectArray;
	delete[] arrOfVertices;
	delete[] arrOfTxtCord;
	//delete[] arrOfNormals;
	delete[] arrOfIndex;
	/*gnrOfVertices = nrOfFaces;*/
	//4ret
}

bool loadHeightMap(char* filename, Heightmap &heightmap) //24 bit colour depth
{

	FILE *fileptr; //filepointer
	BITMAPFILEHEADER bitmapFileHeader; //struct containing file information
	BITMAPINFOHEADER bitmapInfoHeader; //struct contatining image information
	int imageSize, index;
	unsigned char height;

	//Open the file
	fileptr = fopen(filename, "rb");
	if (fileptr == 0)
	{
		return false;
	}

	//Read headers
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fileptr);
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fileptr);

	//retrieve width and height of object
	heightmap.imageWidth = bitmapInfoHeader.biWidth;
	heightmap.imageHeight = bitmapInfoHeader.biHeight;

	//get size of image in bytes
	int padding = heightmap.imageWidth % 4; //Ta det sen
	if (padding > 0)
	{
		padding = 4 - padding;
	}

	imageSize = (heightmap.imageHeight * heightmap.imageWidth * 3) + (heightmap.imageHeight * padding); //3 is for the three values RGB, added 2 byte per row for bumper data.

	//array of image data
	unsigned char* bitmapImage = new unsigned char[imageSize];

	//Find start of image data
	fseek(fileptr, bitmapFileHeader.bfOffBits, SEEK_SET);

	//read data into bitmapimage
	fread(bitmapImage, 1, imageSize, fileptr);

	//close file
	fclose(fileptr);

	//array of vertice positions
	heightmap.verticesPos = new XMFLOAT3[heightmap.imageHeight * heightmap.imageWidth];

	int counter = 0; //Eftersom bilden är i gråskala så är alla värden RGB samma värde, därför läser vi bara R

	gHeightfactor = 25.50f * 1.5f; //mountain smoothing

	//read and put vertex position
	for (int i = 0; i < heightmap.imageHeight; i++)
	{
		for (int j = 0; j < heightmap.imageWidth; j++)
		{
			height = bitmapImage[counter];
			index = (heightmap.imageHeight * i) + j;

			heightmap.verticesPos[index].x = (float)j;
			heightmap.verticesPos[index].y = (float)height / gHeightfactor;
			heightmap.verticesPos[index].z = (float)i;
			counter += 3; //Skipping GB
		}
		counter += padding; //Skip padding info at the end of each row.
	}

	delete[] bitmapImage;
	return true;
}

void setVertexBuffers()
{
	ppVertexBuffers[1] = gVertexBufferMap;
	ppVertexBuffers[0] = gVertexBufferObj;
	gTotalNrOfVert = gnrOfVertices + gnrOfFaces;
}

void updateWorldViewProjection() {
	XMMATRIX World;

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

	XMMATRIX View = XMMatrixLookAtLH(
		camera.GetCamPos(),
		camera.GetCamTarget(),
		camera.GetCamUp()
	);
	View = XMMatrixTranspose(View);

	XMMATRIX Projection = XMMatrixPerspectiveFovLH(
		(float)XM_PI*0.45,
		(float)W_WIDTH / (float)W_HEIGHT,
		0.1f,
		200.0f
	);
	Projection = XMMatrixTranspose(Projection);

	//set each matrix to our cpu side memory
	gWorldMatrix->World = World;
	gWorldMatrix->ViewProjection = XMMatrixMultiply(Projection, View);

	//create a subresource to hold our data while we copy between cpu and gpu memory
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	//copy and map our cpu memory to our gpu buffert
	gDeviceContext->Map(gWorldMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	memcpy(mappedMemory.pData, gWorldMatrix, sizeof(WorldMatrix));
	gDeviceContext->Unmap(gWorldMatrixBuffer, 0);
}

void Render() {
	// clear the back buffer to a deep blue
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Clear the render target buffers.
	for (int i = 0; i < G_BUFFER; i++) {
		gDeviceContext->ClearRenderTargetView(gRenderTargetViewArray[i], clearColor);
	}

	// make sure our depth buffer is cleared to black each time we render
	gDeviceContext->ClearDepthStencilView(gDepth, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Set the sampler state in the pixel shader.
	gDeviceContext->PSSetSamplers(0, 1, &gSampling);

	SetDeferredShaders();

	//bind our constant buffers to coresponding shader
	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorldMatrixBuffer);
	gDeviceContext->PSSetConstantBuffers(0, 1, &gAmbientSpecularBuffer);

	//bind our texture to pixelshader
	//gDeviceContext->PSSetShaderResources(0, 1, &gResource);

	UINT32 vertexSize = sizeof(TriangleVertex);
	UINT32 offset = 0;

	gDeviceContext->Draw(gTotalNrOfVert, 0);
	//gDeviceContext->IASetVertexBuffers(0, 2, ppVertexBuffers, &vertexSize, &offset);
	//gDeviceContext->Draw(gnrOfVertices, 0);


	gDeviceContext->VSSetConstantBuffers(0, 1, &nullCB);
	gDeviceContext->PSSetConstantBuffers(0, 1, &nullCB);

	SetLightShaders();

	gDeviceContext->PSSetConstantBuffers(0, 1, &gCameraMatrixBuffer);
	gDeviceContext->PSSetConstantBuffers(1, 1, &gLightDataBuffer);

	//bind our g-buffer textures to pixelshader
	gDeviceContext->PSSetShaderResources(0, 1, &gShaderResourceViewArray[0]);
	gDeviceContext->PSSetShaderResources(1, 1, &gShaderResourceViewArray[1]);
	gDeviceContext->PSSetShaderResources(2, 1, &gShaderResourceViewArray[2]);
	gDeviceContext->PSSetShaderResources(3, 1, &gShaderResourceViewArray[3]);

	// render each light source
	for (int j = 0; j < nrOfLights; j++) {
		Lights[j].Load(gDeviceContext, gLightDataBuffer);
		gDeviceContext->Draw(gTotalNrOfVert, 0);
	}

	gDeviceContext->PSSetShaderResources(0, 1, &nullSRV[0]);
	gDeviceContext->PSSetShaderResources(1, 1, &nullSRV[0]);
	gDeviceContext->PSSetShaderResources(2, 1, &nullSRV[0]);
	gDeviceContext->PSSetShaderResources(3, 1, &nullSRV[0]);

	gDeviceContext->PSSetConstantBuffers(0, 1, &nullCB);
	gDeviceContext->PSSetConstantBuffers(1, 1, &nullCB);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	//create two timestamps variables and a delta between them to adjust update frequency
	time_point<high_resolution_clock>start = high_resolution_clock::now();
	time_point<high_resolution_clock>end = high_resolution_clock::now();
	duration<double, std::ratio<1, 15>> delta;

	//m_mouse->SetWindow();

	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance); //1. Skapa fönster

	//Mouse and keyboard ini (ONLY MOUSE)
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(wndHandle);
	//Control values
	float rotationValue = 0.01f;

	if (wndHandle) {
		CreateDirect3DContext(wndHandle); //2. Skapa och koppla SwapChain, Device och Device Context

		SetViewport(); //3. Sätt viewport

		CreateShaders(); //4. Skapa vertex- och pixel-shaders

		CreateDeferredQuad();

			Heightmap _heightmap;

			if (!loadHeightMap("maps/height/kon.bmp", _heightmap)) return 404;

			CreateHeightmapData(_heightmap); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout

			delete[] _heightmap.verticesPos;

		LoadObjectFile("hotModel.obj");

		setVertexBuffers();

		CreateConstantBuffer(); //8. Create constant buffers

		CreateSampling();

		CreateLigths();

		ShowWindow(wndHandle, nCmdShow);

		while (WM_QUIT != msg.message) {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {

				//set timestamps and calculate delta between start end end time
				end = high_resolution_clock::now();
				delta = end - start;
				start = high_resolution_clock::now();

				//KEYBOARD 
				{
					auto kb = m_keyboard->GetState();
					if (kb.Escape) {
						camera.SetCamPos(camera.GetCamOriginalPos());
						camera.SetCamTarget(camera.GetCamOriginalTarget());
						camera.SetYawAndPitch(0, 0);
					}

					if (kb.LeftControl && rotationValue > 0) {
						rotationValue = 0.0f;
					}
					else if (kb.LeftShift) {
						rotationValue = 0.01;
					}
					Vector3 moveInDepthCameraClass = Vector3::Zero;
					Vector3 deltaChange = Vector3::Zero;

					//UPDATE VECTOR
					if (kb.W) {//FORWARD IN
						moveInDepthCameraClass += camera.GetCamTarget() - camera.GetCamPos();

					}
					if (kb.S) { //BACK
						moveInDepthCameraClass -= camera.GetCamTarget() - camera.GetCamPos();
					}
					if (kb.D) { //RIGHT
						deltaChange.x += 1.0f;

					}
					if (kb.A) { //LEFT
						deltaChange.x -= 1.0f;
					}
					if (kb.Q) { //UP
						deltaChange.y += 1.0f;
					}
					if (kb.E) { //DOWN
						deltaChange.y -= 1.0f;
					}
					//MOUSE INPUT PRESS LEFTCLICK TO ANGEL
					auto mouse = m_mouse->GetState();

					if (mouse.positionMode == Mouse::MODE_RELATIVE) {
						//MOUSE RELATIVE CONTROL
						float deltaPos[3] = { float(-mouse.x)* ROTATION_GAIN, float(mouse.y)* ROTATION_GAIN, 0.0f };

						camera.UpdateYawAndPitch(deltaPos[0], deltaPos[1]);
					}

					m_mouse->SetMode(mouse.leftButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);


					//UPDATE CAMERA
					{
						deltaChange = XMVector3Normalize(deltaChange);
						//ROTATION OF CAMERA
						deltaChange = deltaChange.x*camera.GetCamRight() + deltaChange.y*camera.GetCamUp();
						deltaChange += moveInDepthCameraClass;

						deltaChange = deltaChange;
						camera.UpdateCamera({ deltaChange.x,deltaChange.y,deltaChange.z }, float(delta.count()));
					}
				}
				//ROTATING WORLD

				//upate rotation depending on time since last update
				rotation += delta.count()*rotationValue;

				//make sure it never goes past 2PI, 
				//sin and cos gets less precise when calculated with higher values
				if (rotation > 2 * XM_PI)
					rotation -= 2 * XM_PI;

				updateWorldViewProjection();

				Render(); //10. Rendera

				gSwapChain->Present(0, 0); //11. Växla front- och back-buffer
			}
		}

		//make sure there are no leaks by releasing stored pointers
		//gVertexBuffer->Release();
		gDeferredQuadBuffer->Release();

		gLightDataBuffer->Release();

		_aligned_free(gAmbientSpecularData);
		gAmbientSpecularBuffer->Release();

		_aligned_free(gCameraMatrix);
		gCameraMatrixBuffer->Release();

		_aligned_free(gWorldMatrix);
		gWorldMatrixBuffer->Release();

		DestroyShaders();

		gDepth->Release();

		for (int i = 0; i < G_BUFFER; i++) {
			if (gRenderTargetViewArray[i]) {
				gRenderTargetViewArray[i]->Release();
			}

			if (gShaderResourceViewArray[i]) {
				gShaderResourceViewArray[i]->Release();
			}

			if (gRenderTargetTextureArray[i]) {
				gRenderTargetTextureArray[i]->Release();
			}
		}

		gSwapChain->Release();
		gDevice->Release();
		gDeviceContext->Release();
		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;
}

HWND InitWindow(HINSTANCE hInstance) {
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"BTH_D3D_DEMO";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, W_WIDTH, W_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"BTH_D3D_DEMO",
		L"BTH Direct3D Demo",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATEAPP:

		Keyboard::ProcessMessage(message, wParam, lParam);
		Mouse::ProcessMessage(message, wParam, lParam);
		break;

	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	}


	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT CreateDepthBuffer(ID3D11Texture2D** pDepthTexture) {
	//describe how the texture should be handled
	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.ArraySize = 1;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	depthDesc.Height = W_HEIGHT;
	depthDesc.Width = W_WIDTH;
	depthDesc.MipLevels = 1;
	depthDesc.MiscFlags = 0;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;

	//load texture data into the texture object, 
	//for now it's a blank texture to be used later as depth buffer
	return gDevice->CreateTexture2D(&depthDesc, NULL, pDepthTexture);
}

HRESULT CreateDepthStencilView(ID3D11Texture2D** pDepthTexture) {
	//describe how the resource should be rendered in view 
	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	ZeroMemory(&depthViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Flags = 0;

	//apply the view description on texture to our global variable
	return gDevice->CreateDepthStencilView(*pDepthTexture, &depthViewDesc, &gDepth);
}

HRESULT CreateRenderTargets() {
	// get the address of the back buffer
	ID3D11Texture2D* pBackBuffer = nullptr;
	gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackbufferRTV);
	pBackBuffer->Release();

	for (int i = 0; i < G_BUFFER; i++) {
		gRenderTargetTextureArray[i] = nullptr;
		gRenderTargetViewArray[i] = nullptr;
		gShaderResourceViewArray[i] = nullptr;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = W_WIDTH;
	textureDesc.Height = W_HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target textures.
	for (int i = 0; i < G_BUFFER; i++) {
		if (FAILED(gDevice->CreateTexture2D(&textureDesc, NULL, &gRenderTargetTextureArray[i]))) {
			return S_FALSE;
		}
	}

	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target views.
	for (int j = 0; j < G_BUFFER; j++) {
		if (FAILED(gDevice->CreateRenderTargetView(gRenderTargetTextureArray[j], &renderTargetViewDesc, &gRenderTargetViewArray[j]))) {
			return S_FALSE;
		}
	}

	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource views.
	for (int k = 0; k < G_BUFFER; k++) {
		if (FAILED(gDevice->CreateShaderResourceView(gRenderTargetTextureArray[k], &shaderResourceViewDesc, &gShaderResourceViewArray[k]))) {
			return S_FALSE;
		}
	}

	//gDeviceContext->OMSetRenderTargets(G_BUFFER, gRenderTargetViewArray, gDepth);

	return S_OK;
}

HRESULT CreateDirect3DContext(HWND wndHandle) {
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = wndHandle;                           // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext
	);

	if (SUCCEEDED(hr)) {
		//pointer to texture data in memory
		ID3D11Texture2D* pDepthTexture = nullptr;

		hr = CreateDepthBuffer(&pDepthTexture);

		if (SUCCEEDED(hr)) {
			hr = CreateDepthStencilView(&pDepthTexture);

			if (SUCCEEDED(hr)) {
				CreateRenderTargets();
			}
		}

		pDepthTexture->Release();
	}

	return hr;
}