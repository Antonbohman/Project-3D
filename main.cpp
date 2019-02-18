#include <windows.h>
#include <chrono>
#include <algorithm>
#include "KeyInput.h"
//#include "bth_image.h"
#include <Mouse.h>

#include <Keyboard.h>
//

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


// window size
#define W_WIDTH 640.0f
#define W_HEIGHT 480.0f

// define number of vertices used in rendering
#define VERTICES 6

using namespace DirectX;
using namespace SimpleMath;
using namespace std::chrono;

// external variables imported from bth_image.h for raw image data
/*extern const unsigned int BTH_IMAGE_WIDTH;
extern const unsigned int BTH_IMAGE_HEIGHT;
extern unsigned char BTH_IMAGE_DATA[];*/

struct Heightmap //Heightmap
{
	int imageWidth; //Width of image file
	int imageHeight; //Height of image file
	XMFLOAT3 *verticesPos; //Array of vertex positions
};

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HRESULT CreateDirect3DContext(HWND wndHandle);

// Most directX Objects are COM Interfaces
// https://es.wikipedia.org/wiki/Component_Object_Model
IDXGISwapChain* gSwapChain = nullptr;

// Device and DeviceContext are the most common objects to
// instruct the API what to do. It is handy to have a reference
// to them available for simple applications.
ID3D11Device* gDevice = nullptr;
ID3D11DeviceContext* gDeviceContext = nullptr;

// A "view" of a particular resource (the color buffer)
ID3D11RenderTargetView* gBackbufferRTV = nullptr;

// a resource to store Vertices in the GPU
ID3D11Buffer* gVertexBuffer = nullptr;

ID3D11InputLayout* gVertexLayout = nullptr;

// resources for depth buffer image and texture image
ID3D11ShaderResourceView* gResource = nullptr;
ID3D11DepthStencilView* gDepth = nullptr;

// resources that represent shaders
ID3D11ComputeShader* gComputeShader = nullptr;
ID3D11VertexShader* gVertexShader = nullptr;
ID3D11GeometryShader* gGeometryShader = nullptr;
ID3D11PixelShader* gPixelShader = nullptr;

int nrOfVertices;
float gHeightfactor;
// resource storing lightning source
struct LightData {
	XMVECTOR ambient;
	XMVECTOR light; //POSITION
	XMVECTOR colour;
	XMVECTOR cameraView;
};
LightData* gLightData = nullptr;
ID3D11Buffer* gLightDataBuffer = nullptr;

// a resource to store world,view,porojection matrix for object in the GPU
struct WorldMatrix {
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};
WorldMatrix* gWorldMatrix = nullptr;
ID3D11Buffer* gWorldMatrixBuffer = nullptr;

//ObjectFile vertexes;
struct ObjInfo
{
	float x, y, z;
	float x_n, y_n, z_n;
	float u, v;
};

ObjInfo* objObject = nullptr;

// CAMERAVIEW
XMVECTOR cameraPosition = { 0.0f, 10.0f, -20.0f, 0.0f };
XMVECTOR cameraOriginalPostion = cameraPosition;
XMVECTOR cameraFocus = { 0.0f, 0.0f, 0.0f, 0.0f };// XMVECTOR camTarget;
XMVECTOR cameraOriginalFocus = cameraFocus;
XMVECTOR cameraUp = { 0.0f,1.0f,0.0f,0.0f };

XMMATRIX camRotationMatrix;

float pitch;
float yaw;

#define ROTATION_GAIN 0.004f
#define MOVEMENT_GAIN 0.07f


//CAMERA MANIPULATION
XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);



//https://www.braynzarsoft.net/viewtutorial/q16390-19-first-person-camera

//XMMATRIX camRotationMatrix;
//XMMATRIX groundWorld;
//
//float moveLeftRight = 0.0f;
//float moveBackForward = 0.0f;
//float moveDownUp = 0.0f;
//
//float camYaw = 0.0f;
//float camPitch = 0.0f;
//
//XMVECTOR camPosition;
//XMMATRIX camView;
//
//
//void UpdateCamera()
//{
//	//update target 
//	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
//	cameraFocus = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
//	cameraFocus = XMVector3Normalize(cameraFocus);
//
//	//FIRST PERSON
//	//XMMATRIX RotateYTempMatrix;
//	//RotateYTempMatrix = XMMatrixRotationY(camYaw);
//
//	//camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
//	//camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix); //DECLARE
//	//camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);
//
//	//FREE-LOOK camera
//	camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
//	camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
//	cameraUp = XMVector3Cross(camForward, camRight);
//
//
//	camPosition = moveLeftRight * camRight; 
//	camPosition += moveBackForward * camForward;
//
//	moveLeftRight = 0.0f;
//	moveBackForward = 0.0f;
//	moveDownUp = 0.0f; //not implemented
//
//	cameraFocus = camPosition + cameraFocus;
//
//	camView = XMMatrixLookAtLH(camPosition, cameraFocus, cameraUp);
//}
//
//void Input(double deltaT)
//{
//	float movementSpeed = 15.0f*deltaT;
//
//	//CHANGE CAMERA POSITION without delta
//		//movement in Z-axis W+ S-
//	if (KeyInput(Wkey))
//	{
//		moveBackForward += movementSpeed;
//	}
//	if (KeyInput(Skey))
//	{
//		moveBackForward -= movementSpeed;
//	}
//	//movement in X-axis D+ A-
//	if (KeyInput(Akey))
//	{
//		moveLeftRight -= movementSpeed;
//	}
//	if (KeyInput(Dkey))
//	{
//		moveLeftRight += movementSpeed;
//	}
//	//movement in Y-axis Q+ E-
//	if (KeyInput(Qkey))
//	{
//		cameraPosition += {0.001f, 0.0f, 0.0f, 0.0f};
//		cameraFocus += {0.001f, 0.0f, 0.0f, 0.0f};
//	}
//	if (KeyInput(Ekey))
//	{
//		cameraPosition -= {0.001f, 0.0f, 0.0f, 0.0f};
//		cameraFocus -= {0.001f, 0.0f, 0.0f, 0.0f};
//	}
//	//rest position with HOME
//	if (KeyInput(Homekey))
//	{
//		cameraPosition = cameraOriginalPostion;
//	}
//	//rest position with HOME
//	if (KeyInput(Homekey))
//	{
//		cameraFocus = cameraOriginalFocus;
//	}
//}

// keeping track of current rotation
float rotation = 1.5f*XM_PI;

HRESULT CreateShaders() {
	// Binary Large OBject (BLOB), for compiled shader, and errors.
	ID3DBlob* pCS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	/*HRESULT result = D3DCompileFromFile(
		L"Compute.hlsl",   // filename
		nullptr,		  // optional macros
		nullptr,		  // optional include files
		"VS_main",		  // entry point
		"vs_5_0",		  // shader model (target)
		D3DCOMPILE_DEBUG, // shader compile options (DEBUGGING)
		0,				  // IGNORE...DEPRECATED.
		&pCS,			  // double pointer to ID3DBlob
		&errorBlob		  // pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result)) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pCS)
			pCS->Release();
		return result;
	}

	gDevice->CreateComputeShader(
		pCS->GetBufferPointer(),
		pCS->GetBufferSize(),
		nullptr,
		&gComputeShader
	);

	pCS->Release();*/

	ID3DBlob* pVS = nullptr;

	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	HRESULT result = D3DCompileFromFile(
		L"Vertex.hlsl",   // filename
		nullptr,		  // optional macros
		nullptr,		  // optional include files
		"VS_main",		  // entry point
		"vs_5_0",		  // shader model (target)
		D3DCOMPILE_DEBUG, // shader compile options (DEBUGGING)
		0,				  // IGNORE...DEPRECATED.
		&pVS,			  // double pointer to ID3DBlob		
		&errorBlob		  // pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result)) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	gDevice->CreateVertexShader(
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&gVertexShader
	);

	// create input layout (verified using vertex shader)
	// Press F1 in Visual Studio with the cursor over the datatype to jump
	// to the documentation online!
	// please read:
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205117(v=vs.85).aspx
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",					 // "semantic" name in shader
			0,							 // "semantic" index (not used)
			DXGI_FORMAT_R32G32B32_FLOAT, // size of ONE element (3 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
		{
			"COLOR",
			0,							 // same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,// offset of FIRST element (after POSITION)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"UV_POS",
			0,							// same slot as previous (same vertexBuffer)
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,// offset of FIRST element (after COLOR)
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout);

	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	ID3DBlob* pGS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"Geometry.hlsl",	// filename
		nullptr,			// optional macros
		nullptr,			// optional include files
		"main",				// entry point
		"gs_5_0",			// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,					// effect compile options
		&pGS,				// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result)) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pGS)
			pGS->Release();
		return result;
	}

	gDevice->CreateGeometryShader(
		pGS->GetBufferPointer(),
		pGS->GetBufferSize(),
		nullptr,
		&gGeometryShader
	);

	pGS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	if (errorBlob) errorBlob->Release();
	errorBlob = nullptr;

	result = D3DCompileFromFile(
		L"Fragment.hlsl",   // filename
		nullptr,		    // optional macros
		nullptr,		    // optional include files
		"PS_main",		    // entry point
		"ps_5_0",		    // shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				    // effect compile options
		&pPS,			    // double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	// compilation failed?
	if (FAILED(result)) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			// release "reference" to errorBlob interface object
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}

	gDevice->CreatePixelShader(
		pPS->GetBufferPointer(),
		pPS->GetBufferSize(),
		nullptr,
		&gPixelShader
	);

	// we do not need anymore this COM object, so we release it.
	pPS->Release();

	return S_OK;
}

struct TriangleVertex {
	float x, y, z;
	float r, g, b;
	float u, v;
};

int gnrOfVertices = 0;

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

	TriangleVertex* triangleVertices = new TriangleVertex[gnrOfVertices];
	//TriangleVertex triangleVertices[3820];

	int vertNr = 0;

	for (int i = 0; i < heightmap.imageHeight - 1; i++)
	{
		int X = (i * heightmap.imageWidth);
		int Y = ((i + 1) * heightmap.imageWidth);

		for (int k = 0/*i * heightmap.imageWidth*/; k < /*(i + 1) **/ heightmap.imageWidth - 1; k++)
		{
			/*Position*/
			triangleVertices[vertNr].x = heightmap.verticesPos[Y].x - (heightmap.imageWidth / 2);
			triangleVertices[vertNr].y = heightmap.verticesPos[Y].y;
			triangleVertices[vertNr].z = heightmap.verticesPos[Y].z - (heightmap.imageHeight / 2);
			/*Colour*/
			if (triangleVertices[vertNr].y > grass.w)
			{
				if (triangleVertices[vertNr].y > mountain.w)
				{
					if (triangleVertices[vertNr].y > snow.w)
					{
						triangleVertices[vertNr].r = snow.x;
						triangleVertices[vertNr].g = snow.y;
						triangleVertices[vertNr].b = snow.z;
					}
					else
					{
						triangleVertices[vertNr].r = mountain.x;
						triangleVertices[vertNr].g = mountain.y;
						triangleVertices[vertNr].b = mountain.z;
					}
				}
				else
				{
					triangleVertices[vertNr].r = grass.x;
					triangleVertices[vertNr].g = grass.y;
					triangleVertices[vertNr].b = grass.z;
				}
			}
			else
			{
				triangleVertices[vertNr].r = bedrock.x;
				triangleVertices[vertNr].g = bedrock.y;
				triangleVertices[vertNr].b = bedrock.z;
			}
			//triangleVertices[vertNr].r = heightmap.verticesPos[Y].y;
			//triangleVertices[vertNr].g = heightmap.verticesPos[Y].y;
			//triangleVertices[vertNr].b = heightmap.verticesPos[Y].y;

			/*UV*/
			triangleVertices[vertNr].u = 0.0f;
			triangleVertices[vertNr].v = 1.0f;

			vertNr++;

			/*..............................*/

			/*Position*/
			triangleVertices[vertNr].x = heightmap.verticesPos[X + 1].x - (heightmap.imageWidth / 2);
			triangleVertices[vertNr].y = heightmap.verticesPos[X + 1].y;
			triangleVertices[vertNr].z = heightmap.verticesPos[X + 1].z - (heightmap.imageHeight / 2);
			/*Colour*/
			if (triangleVertices[vertNr].y > grass.w)
			{
				if (triangleVertices[vertNr].y > mountain.w)
				{
					if (triangleVertices[vertNr].y > snow.w)
					{
						triangleVertices[vertNr].r = snow.x;
						triangleVertices[vertNr].g = snow.y;
						triangleVertices[vertNr].b = snow.z;
					}
					else
					{
						triangleVertices[vertNr].r = mountain.x;
						triangleVertices[vertNr].g = mountain.y;
						triangleVertices[vertNr].b = mountain.z;
					}
				}
				else
				{
					triangleVertices[vertNr].r = grass.x;
					triangleVertices[vertNr].g = grass.y;
					triangleVertices[vertNr].b = grass.z;
				}
			}
			else
			{
				triangleVertices[vertNr].r = bedrock.x;
				triangleVertices[vertNr].g = bedrock.y;
				triangleVertices[vertNr].b = bedrock.z;
			}
			//triangleVertices[vertNr].r = heightmap.verticesPos[Y].y;
			//triangleVertices[vertNr].g = heightmap.verticesPos[Y].y;
			//triangleVertices[vertNr].b = heightmap.verticesPos[Y].y;

			/*UV*/
			triangleVertices[vertNr].u = 1.0f;
			triangleVertices[vertNr].v = 1.0f;

			vertNr++;

			/*-------------------------------*/

			/*Position*/
			triangleVertices[vertNr].x = heightmap.verticesPos[X].x - (heightmap.imageWidth / 2);
			triangleVertices[vertNr].y = heightmap.verticesPos[X].y;
			triangleVertices[vertNr].z = heightmap.verticesPos[X].z - (heightmap.imageHeight / 2);
			/*Colour*/
			if (triangleVertices[vertNr].y > grass.w)
			{
				if (triangleVertices[vertNr].y > mountain.w)
				{
					if (triangleVertices[vertNr].y > snow.w)
					{
						triangleVertices[vertNr].r = snow.x;
						triangleVertices[vertNr].g = snow.y;
						triangleVertices[vertNr].b = snow.z;
					}
					else
					{
						triangleVertices[vertNr].r = mountain.x;
						triangleVertices[vertNr].g = mountain.y;
						triangleVertices[vertNr].b = mountain.z;
					}
				}
				else
				{
					triangleVertices[vertNr].r = grass.x;
					triangleVertices[vertNr].g = grass.y;
					triangleVertices[vertNr].b = grass.z;
				}
			}
			else
			{
				triangleVertices[vertNr].r = bedrock.x;
				triangleVertices[vertNr].g = bedrock.y;
				triangleVertices[vertNr].b = bedrock.z;
			}
			//triangleVertices[vertNr].r = heightmap.verticesPos[Y].y;
			//triangleVertices[vertNr].g = heightmap.verticesPos[Y].y;
			//triangleVertices[vertNr].b = heightmap.verticesPos[Y].y;

			/*UV*/
			triangleVertices[vertNr].u = 0.0f;
			triangleVertices[vertNr].v = 0.0f;


			vertNr++;
			X++;

			/*-------------------------------*/
			/*Next triangle*/

			/*Position*/
			triangleVertices[vertNr].x = heightmap.verticesPos[Y].x - (heightmap.imageWidth / 2);
			triangleVertices[vertNr].y = heightmap.verticesPos[Y].y;
			triangleVertices[vertNr].z = heightmap.verticesPos[Y].z - (heightmap.imageHeight / 2);
			/*Colour*/
			if (triangleVertices[vertNr].y > grass.w)
			{
				if (triangleVertices[vertNr].y > mountain.w)
				{
					if (triangleVertices[vertNr].y > snow.w)
					{
						triangleVertices[vertNr].r = snow.x;
						triangleVertices[vertNr].g = snow.y;
						triangleVertices[vertNr].b = snow.z;
					}
					else
					{
						triangleVertices[vertNr].r = mountain.x;
						triangleVertices[vertNr].g = mountain.y;
						triangleVertices[vertNr].b = mountain.z;
					}
				}
				else
				{
					triangleVertices[vertNr].r = grass.x;
					triangleVertices[vertNr].g = grass.y;
					triangleVertices[vertNr].b = grass.z;
				}
			}
			else
			{
				triangleVertices[vertNr].r = bedrock.x;
				triangleVertices[vertNr].g = bedrock.y;
				triangleVertices[vertNr].b = bedrock.z;
			}
			//triangleVertices[vertNr].r = heightmap.verticesPos[Y].y;
			//triangleVertices[vertNr].g = heightmap.verticesPos[Y].y;
			//triangleVertices[vertNr].b = heightmap.verticesPos[Y].y;

			/*UV*/
			triangleVertices[vertNr].u = 1.0f;
			triangleVertices[vertNr].v = 1.0f;

			vertNr++;

			/*..............................*/

			/*Position*/
			triangleVertices[vertNr].x = heightmap.verticesPos[X + heightmap.imageWidth].x - (heightmap.imageWidth / 2);
			triangleVertices[vertNr].y = heightmap.verticesPos[X + heightmap.imageWidth].y;
			triangleVertices[vertNr].z = heightmap.verticesPos[X + heightmap.imageWidth].z - (heightmap.imageHeight / 2);
			/*Colour*/
			if (triangleVertices[vertNr].y > grass.w)
			{
				if (triangleVertices[vertNr].y > mountain.w)
				{
					if (triangleVertices[vertNr].y > snow.w)
					{
						triangleVertices[vertNr].r = snow.x;
						triangleVertices[vertNr].g = snow.y;
						triangleVertices[vertNr].b = snow.z;
					}
					else
					{
						triangleVertices[vertNr].r = mountain.x;
						triangleVertices[vertNr].g = mountain.y;
						triangleVertices[vertNr].b = mountain.z;
					}
				}
				else
				{
					triangleVertices[vertNr].r = grass.x;
					triangleVertices[vertNr].g = grass.y;
					triangleVertices[vertNr].b = grass.z;
				}
			}
			else
			{
				triangleVertices[vertNr].r = bedrock.x;
				triangleVertices[vertNr].g = bedrock.y;
				triangleVertices[vertNr].b = bedrock.z;
			}
			//triangleVertices[vertNr].r = heightmap.verticesPos[Y].y;
			//triangleVertices[vertNr].g = heightmap.verticesPos[Y].y;
			//triangleVertices[vertNr].b = heightmap.verticesPos[Y].y;

			/*UV*/
			triangleVertices[vertNr].u = 1.0f;
			triangleVertices[vertNr].v = 0.0f;

			vertNr++;

			/*-------------------------------*/

			/*Position*/
			triangleVertices[vertNr].x = heightmap.verticesPos[X].x - (heightmap.imageWidth / 2);
			triangleVertices[vertNr].y = heightmap.verticesPos[X].y;
			triangleVertices[vertNr].z = heightmap.verticesPos[X].z - (heightmap.imageHeight / 2);
			/*Colour*/
			if (triangleVertices[vertNr].y > grass.w)
			{
				if (triangleVertices[vertNr].y > mountain.w)
				{
					if (triangleVertices[vertNr].y > snow.w)
					{
						triangleVertices[vertNr].r = snow.x;
						triangleVertices[vertNr].g = snow.y;
						triangleVertices[vertNr].b = snow.z;
					}
					else
					{
						triangleVertices[vertNr].r = mountain.x;
						triangleVertices[vertNr].g = mountain.y;
						triangleVertices[vertNr].b = mountain.z;
					}
				}
				else
				{
					triangleVertices[vertNr].r = grass.x;
					triangleVertices[vertNr].g = grass.y;
					triangleVertices[vertNr].b = grass.z;
				}
			}
			else
			{
				triangleVertices[vertNr].r = bedrock.x;
				triangleVertices[vertNr].g = bedrock.y;
				triangleVertices[vertNr].b = bedrock.z;
			}
			//triangleVertices[vertNr].r = heightmap.verticesPos[Y].y;
			//triangleVertices[vertNr].g = heightmap.verticesPos[Y].y;
			//triangleVertices[vertNr].b = heightmap.verticesPos[Y].y;

			/*UV*/
			triangleVertices[vertNr].u = 0.0f;
			triangleVertices[vertNr].v = 0.0f;

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

	//int size = sizeof(triangleVertices);

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)triangleVertices;

	// create a Vertex Buffer
	HRESULT error;
	error = gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
	delete triangleVertices;
}

void CreateTriangleData() {
	// Array of Structs (AoS)
	TriangleVertex triangleVertices[VERTICES]
	{
		-0.5f, -0.5f, 0.0f,	//v0 pos
		1.0f, 0.0f, 0.0f,	//v0 color
		0.0f, 1.0f,			//v0 uv

		-0.5f, 0.5f, 0.0f,	//v1 pos
		0.0f, 1.0f, 0.0f,	//v1 color
		0.0f, 0.0f,			//v1 uv

		0.5f, -0.5f, 0.0f,  //v2 pos
		0.0f, 0.0f, 1.0f,	//v2 color
		1.0f, 1.0f,			//v2 uv

		//----//

		0.5f, 0.5f, 0.0f,	//v3 pos
		1.0f, 1.0f, 0.0f,	//v3 color
		1.0f, 0.0f,			//v3 uv

		0.5f, -0.5f, 0.0f,	//v4 pos
		0.0f, 0.0f, 1.0f,	//v4 color
		1.0f, 1.0f,			//v4 uv

		-0.5f, 0.5f, 0.0f,  //v5 pos
		0.0f, 1.0f, 0.0f,   //v5 color
		0.0f, 0.0f			//v5 uv
	};

	// Describe the Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	// what type of buffer will this be?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// what type of usage (press F1, read the docs)
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = sizeof(triangleVertices);

	int size = sizeof(triangleVertices);

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;

	// create a Vertex Buffer
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
	delete triangleVertices;
}

void CreateConstantBuffer() {
	//allocate space in memory aligned to a multitude of 16
	gLightData = (LightData*)_aligned_malloc(sizeof(LightData), 16);

	//set our faked light source values, 
	//since we won't be updating these values while program is running
	gLightData->ambient = XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f);
	gLightData->light = XMVectorSet(0.0f, 50.0f, -30.0f, 1.0f);
	gLightData->colour = XMVectorSet(1.0f, 1.0f, 1.0f, 50.0f);
	gLightData->cameraView = cameraPosition;

	//create a description objekt defining how the buffer should be handled
	D3D11_BUFFER_DESC lightDesc;
	ZeroMemory(&lightDesc, sizeof(lightDesc));
	lightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightDesc.ByteWidth = sizeof(LightData);
	lightDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//bind light data to a subresource
	D3D11_SUBRESOURCE_DATA lightData;
	ZeroMemory(&lightData, sizeof(lightData));
	lightData.pSysMem = gLightData;
	lightData.SysMemPitch = 0;
	lightData.SysMemSlicePitch = 0;

	//create buffer for light subresource
	gDevice->CreateBuffer(&lightDesc, &lightData, &gLightDataBuffer);

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

void CreateTextureResource() {
	//pointer to texture data in memory
	ID3D11Texture2D* pTextureData;

	//describe how the texture should be handled
	D3D11_TEXTURE2D_DESC textureDesc;
	//textureDesc.Width = BTH_IMAGE_WIDTH;
	//textureDesc.Height = BTH_IMAGE_HEIGHT;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MiscFlags = 0;
	textureDesc.CPUAccessFlags = 0;

	//bind texture to a subresource
	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	//subData.pSysMem = (void*)BTH_IMAGE_DATA;
	//subData.SysMemPitch = BTH_IMAGE_WIDTH * 4 * sizeof(char);

	//load texture data into the texture object
	gDevice->CreateTexture2D(&textureDesc, &subData, &pTextureData);

	//describe how the resource should be rendered in view 
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	resourceViewDesc.Format = textureDesc.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;

	//apply the view description on texture to our global variable
	gDevice->CreateShaderResourceView(pTextureData, &resourceViewDesc, &gResource);

	//release our pointer
	pTextureData->Release();
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
	int nrOfTxtCord;

	bool normalStart = false;
	XMFLOAT3* arrOfNormals = nullptr;
	int nrOfNormals;

	bool indexStart = false;
	XMINT3* arrOfIndex = nullptr;
	int nrOfIndex;
	int indexArrSize = 0;

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
					arrOfTxtCord = new XMFLOAT2[nrOfVert];
					textureCordStart = true;
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
					arrOfNormals = new XMFLOAT3[nrOfNormals];
					normalStart = true;
				}

				XMFLOAT3 vertex;
				fscanf(fileptr, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

				arrOfNormals[nrOfNormals] = vertex;
				nrOfNormals++;
			}
			else if (strcmp(line, "f") == 0)
			{
				if (indexStart == false)
				{
					indexArrSize = (nrOfVert * 1.5);
					arrOfIndex = new XMINT3[indexArrSize];
					normalStart = true;
				}
				if (nrOfIndex == indexArrSize)
				{
					XMINT3* tempArr = new XMINT3[arrSize + 50];
					for (int i = 0; i < arrSize; i++)
					{
						tempArr[i] = arrOfIndex[i];
					}
					delete arrOfIndex;
					arrOfIndex = tempArr;

					indexArrSize += 50;
				}

			}
		}
	}

	objObject = new ObjInfo[nrOfVert];
	for (int i = 0; i < nrOfVert; i++)
	{
		objObject[i].x = arrOfVertices[i].x;
		objObject[i].y = arrOfVertices[i].y;
		objObject[i].z = arrOfVertices[i].z;

		objObject[i].x_n = arrOfNormals[i].x;
		objObject[i].y_n = arrOfNormals[i].y;
		objObject[i].z_n = arrOfNormals[i].z;

		objObject[i].u = arrOfTxtCord[i].x;
		objObject[i].v = arrOfTxtCord[i].y;
	}
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

	//int vertNr = 0;
	//XMFLOAT3 test = 
	//{ 
	//	0, 0, 0
	//};
	//for (int i = 600; i < imageSize; i++)
	//{
	//	vertNr = bitmapImage[i];

	//	test.x = vertNr+1-1;
	//}

	//close file
	fclose(fileptr);

	//array of vertice positions
	heightmap.verticesPos = new XMFLOAT3[heightmap.imageHeight * heightmap.imageWidth];

	int counter = 0; //Eftersom bilden är i gråskala så är alla värden RGB samma värde, därför läser vi bara R

	gHeightfactor = 25.50f * 0.5f; //mountain smoothing

	//read and put vertex position
	for (int i = 0; i < heightmap.imageHeight; i++)
	{
		for (int j = 0; j < heightmap.imageWidth; j++)
		{
			height = bitmapImage[counter];
			index = (heightmap.imageHeight * i) + j;

			heightmap.verticesPos[index].x = (float)j;
			//if (height < 0) height = 0;
			heightmap.verticesPos[index].y = (float)height / gHeightfactor;
			heightmap.verticesPos[index].z = (float)i;
			//test = heightmap.verticesPos[index];
			counter += 3;
		}
		counter += padding; //Skip bumper info at the end of each row.
	}

	delete[] bitmapImage;
	return true;
}

void Render() {
	// clear the back buffer to a deep blue
	float clearColor[] = { 0, 0, 0, 1 };

	// use DeviceContext to talk to the API
	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);

	// make sure our depth buffer is cleared to black each time we render
	gDeviceContext->ClearDepthStencilView(gDepth, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//bind our constant buffers to coresponding shader
	gDeviceContext->GSSetConstantBuffers(0, 1, &gWorldMatrixBuffer);
	gDeviceContext->PSSetConstantBuffers(0, 1, &gLightDataBuffer);

	//bind our texture to pixelshader
	//gDeviceContext->PSSetShaderResources(0, 1, &gResource);


	// specifying NULL or nullptr we are disabling that stage
	// in the pipeline
	//gDeviceContext->CSSetShader(gComputeShader, nullptr, 0);
	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShader, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

	UINT32 vertexSize = sizeof(TriangleVertex);
	UINT32 offset = 0;

	// specify which vertex buffer to use next.
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);

	// specify the topology to use when drawing
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// specify the IA Layout (how is data passed)
	gDeviceContext->IASetInputLayout(gVertexLayout);

	// issue a draw call of 3 vertices (similar to OpenGL)
	gDeviceContext->Draw(gnrOfVertices, 0);
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
	float rotationValue=0.01f;
	bool orbital = true;

	if (wndHandle) {
		CreateDirect3DContext(wndHandle); //2. Skapa och koppla SwapChain, Device och Device Context

		SetViewport(); //3. Sätt viewport

		CreateShaders(); //4. Skapa vertex- och pixel-shaders

		Heightmap _heightmap;

		if (!loadHeightMap("kon.bmp", _heightmap)) return 404;

		CreateHeightmapData(_heightmap); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout
		//CreateTriangleData(); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout

		delete[] _heightmap.verticesPos;

		CreateConstantBuffer(); //8. Create constant buffers

		//CreateTextureResource(); //9. Create and store texture image



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
				
				//NEW CONTROLS 
				//KEYBOARD
				auto kb = m_keyboard->GetState();
				if (kb.Escape) {
					cameraPosition = cameraOriginalPostion;
					cameraFocus = cameraOriginalFocus;
					pitch = yaw = 0;

				}
				Vector3 move = Vector3::Zero;
				Vector3 smartMove = Vector3::Zero;
				Vector4 toAdd =Vector4::Zero ;
				//UPDATE VECTOR
				if (kb.W) {//FORWARD IN
					move.z += 1.0f;  //IN Z
					toAdd += cameraFocus - cameraPosition; //till fokus
					
				}
				if (kb.S) { //BACK
					move.z -= 1.0f; //UT Z
					toAdd -= cameraFocus - cameraPosition; //från fokus
				}
				if (kb.D) { //RIGHT
					move.x += 1.0f;
				
				}
				if (kb.A) { //LEFT
					move.x -= 1.0f;
					
				}
				if (kb.Q) { //UP
					move.y += 1.0f;
					
				}
				if (kb.E) { //DOWN
					move.y -= 1.0f;
					
				}

				//MOUSE INPUT PRESS LEFTCLICK TO ANGEL
				auto mouse = m_mouse->GetState();

				if (mouse.positionMode == Mouse::MODE_RELATIVE) {
					float deltaPos[3] = { float(mouse.x)* ROTATION_GAIN, float(mouse.y)* ROTATION_GAIN, 0.0f };

					pitch -= deltaPos[1]; //Y
					yaw -= deltaPos[0]; //X

					float limit = XM_PI / 2.0f - 0.2f;
					pitch = max(-limit, pitch);
					pitch = min(+limit, pitch);

					if (yaw > XM_PI)
					{
						yaw -= XM_PI * 2.0f;
					}
					else if (yaw < -XM_PI)
					{
						yaw += XM_PI * 2.0f;
					}

				}
				m_mouse->SetMode(mouse.leftButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);

				//https://www.braynzarsoft.net/viewtutorial/q16390-19-first-person-camera
				
				//UPDATE CAMERA
				{
					//ROTATION OF CAMERA
					camRotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, 0);
					//cameraFocus = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
					//cameraFocus = XMVector3Normalize(cameraFocus); //DID NOT WORK

					//DID WORK
					float y = sinf(pitch);
					float r = cosf(pitch);
					float z = r * cosf(yaw);
					float x = r * sinf(yaw);

					cameraFocus = cameraPosition + XMVECTOR{ x, y, z, 0.0f };
					//cameraFocus = XMVector3Normalize(cameraFocus); 

					XMMATRIX RotateYTempMatrix;
					RotateYTempMatrix = XMMatrixRotationY(yaw);

					//Update cameraForward,Up,Right
					camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
					camRight = XMVector3Normalize(camRight);
					cameraUp = XMVector3TransformCoord(cameraUp, RotateYTempMatrix);
					camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

					start = high_resolution_clock::now();
					//NOT FINISHED SOON DONE
					move = move * float(delta.count());
					toAdd = toAdd * float(delta.count());
					toAdd += move.x*camRight + move.y*cameraUp /*+ move.z*camForward*/;

					cameraPosition += toAdd;
				}

				//ROTATING WORLD
				
				//upate rotation depending on time since last update
				rotation += delta.count()*rotationValue;
				
				//make sure it never goes past 2PI, 
				//sin and cos gets less precise when calculated with higher values
				if (rotation > 2 * XM_PI)
				rotation -= 2 * XM_PI;
				
				XMMATRIX World;

				//alternativly use XMMatrixRotationX(rotation);
				XMMATRIX WorldX = XMMatrixSet(
					1.0, 0.0, 0.0, 0.0,
					0.0, (float)cos(rotation), (float)sin(rotation), 0.0,
					0.0, (float)-sin(rotation), (float)cos(rotation), 0.0,
					0.0, 0.0, 0.0, 1.0
				);

				//alternativly use XMMatrixRotationY(rotation);
				XMMATRIX WorldY = XMMatrixSet(
					(float)cos(rotation), 0.0, (float)-sin(rotation), 0.0,
					0.0, 1.0, 0.0, 0.0,
					(float)sin(rotation), 0.0, (float)cos(rotation), 0.0,
					0.0, 0.0, 0.0, 1.0
				);
				//alternativly use XMMatrixRotationZ(rotation); 
				XMMATRIX WorldZ = XMMatrixSet(
					(float)cos(rotation), (float)sin(rotation), 0.0, 0.0,
					(float)-sin(rotation), (float)cos(rotation), 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0
				);

				World = WorldY;
				//World = XMMatrixMultiply(WorldX, World);
				//World = XMMatrixMultiply(World, WorldZ);

				XMMATRIX View = XMMatrixLookAtLH(
					/*{ 0.0f, 10.0f, -20.0f, 0.0f },*/
					cameraPosition,
					cameraFocus,
					cameraUp
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
				gWorldMatrix->world = World;
				gWorldMatrix->view = View;
				gWorldMatrix->projection = Projection;

				//create a subresource to hold our data while we copy between cpu and gpu memory
				D3D11_MAPPED_SUBRESOURCE mappedMemory;

				//copy and map our cpu memory to our gpu buffert
				gDeviceContext->Map(gWorldMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
				memcpy(mappedMemory.pData, gWorldMatrix, sizeof(WorldMatrix));
				gDeviceContext->Unmap(gWorldMatrixBuffer, 0);

				Render(); //10. Rendera

				gSwapChain->Present(0, 0); //11. Växla front- och back-buffer
			}
		}

		//make sure there are no leaks by releasing stored pointers
		gVertexBuffer->Release();

		_aligned_free(gLightData);
		gLightDataBuffer->Release();

		_aligned_free(gWorldMatrix);
		gWorldMatrixBuffer->Release();

		gVertexLayout->Release();
		gVertexShader->Release();
		gGeometryShader->Release();
		gPixelShader->Release();

		//gResource->Release();
		gDepth->Release();

		gBackbufferRTV->Release();
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
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext);

	if (SUCCEEDED(hr)) {
		//pointer to texture data in memory
		ID3D11Texture2D* pDepthTexture = nullptr;

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
		HRESULT error = gDevice->CreateTexture2D(&depthDesc, NULL, &pDepthTexture);

		if (SUCCEEDED(error)) {
			//describe how the resource should be rendered in view 
			D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
			ZeroMemory(&depthViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			depthViewDesc.Format = depthDesc.Format;
			depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthViewDesc.Flags = 0;

			//apply the view description on texture to our global variable
			gDevice->CreateDepthStencilView(pDepthTexture, &depthViewDesc, &gDepth);

			//release our pointer
			pDepthTexture->Release();
		}

		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackbufferRTV);
		pBackBuffer->Release();

		// set the render target as the back buffer and add depth buffer target
		gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, gDepth);
	}

	return hr;
}