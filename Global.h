#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "LightSource.h"
#include "Camera.h"

using namespace DirectX;

///////////////////
// Defines       //
///////////////////

// window size
#define W_WIDTH 640.0f
#define W_HEIGHT 480.0f
#define FOV 0.45f

// define number of vertices used in rendering
#define VERTICES 6

// light types
#define L_POINT 0
#define L_SPOT 1
#define L_DIRECTIONAL 2

// define number of layers of g-buffer used in rendering
#define G_BUFFER 4



///////////////////
// Structs       //
///////////////////

// heightmap
struct Heightmap {
	int imageWidth;			//Width of image file
	int imageHeight;		//Height of image file
	XMFLOAT3 *verticesPos;  //Array of vertex positions
};

// resource storing ambient and specular
struct AmbientSpecular {
	XMVECTOR Ambient;
	XMVECTOR Specular;
};

// resource storing camera source
struct CameraMatrix {
	XMVECTOR Origin;
	XMVECTOR Focus;
};

// a resource to store world,view,porojection matrix for object in the GPU
struct WorldMatrix {
	XMMATRIX World;
	XMMATRIX ViewProjection;
};

// basic vertex struct
struct TriangleVertex {
	float x, y, z;
	float r, g, b;
	float u, v;
};

// basic vertex struct
struct PositionVertex {
	float x, y;
};

// resource for storing a light
struct LightData {
	int type;
	int byte2;
	XMVECTOR position;
	XMVECTOR direction;
	XMVECTOR colour;
	float intensity;
	float angleX; float angleY;
};



/////////////////////
// Variables       //
/////////////////////

// Most directX Objects are COM Interfaces
// https://es.wikipedia.org/wiki/Component_Object_Model
extern IDXGISwapChain* gSwapChain;

// Device and DeviceContext are the most common objects to
// instruct the API what to do. It is handy to have a reference
// to them available for simple applications.
extern ID3D11Device* gDevice;
extern ID3D11DeviceContext* gDeviceContext;

// sampling resource
extern ID3D11SamplerState* gSampling;

// a resource to store Vertices in the GPU
//extern ID3D11Buffer* gVertexBufferObj;
//extern ID3D11Buffer* gVertexBufferMap;
//extern TriangleVertex* gMap;
//extern TriangleVertex* gObject;
extern int gnrOfVert[5];
extern ID3D11Buffer*ppVertexBuffers[5];
extern int nrOfVertexBuffers;
//extern int gTotalNrOfVert;
extern char* textureName;

extern ID3D11Buffer* gDeferredQuadBuffer;

// input layouts
extern ID3D11InputLayout* gVertexLayout;
extern ID3D11InputLayout* gLightVertexLayout;

// render targets
extern ID3D11RenderTargetView* gBackbufferRTV;
extern ID3D11Texture2D* gRenderTargetTextureArray[G_BUFFER];
extern ID3D11RenderTargetView* gRenderTargetViewArray[G_BUFFER];
extern ID3D11ShaderResourceView* gShaderResourceViewArray[G_BUFFER];

// resources for depth buffer image
extern ID3D11DepthStencilView* gDepth;

// resources that represent shaders
extern ID3D11VertexShader* gVertexShader;
extern ID3D11GeometryShader* gGeometryShader;
extern ID3D11PixelShader* gPixelShader;
extern ID3D11ComputeShader* gComputeShader;
extern ID3D11VertexShader* gLightVertexShader;
extern ID3D11PixelShader* gLightPixelShader;

extern AmbientSpecular* gAmbientSpecularData;
extern ID3D11Buffer* gAmbientSpecularBuffer;

extern CameraMatrix* gCameraMatrix;
extern ID3D11Buffer* gCameraMatrixBuffer;

extern WorldMatrix* gWorldMatrix;
extern ID3D11Buffer* gWorldMatrixBuffer;

extern ID3D11Buffer* gLightDataBuffer;

extern LightSource* Lights;
extern int nrOfLights;

// CAMERAVIEW
extern XMVECTOR CameraView;
extern Camera camera;

// keeping track of current rotation
extern float rotation;

extern int nrOfVertices;
//extern float gHeightfactor;

extern int gnrOfVertices;

//clear pointers
extern ID3D11ShaderResourceView* nullSRV[1];
extern ID3D11Buffer* nullCB;