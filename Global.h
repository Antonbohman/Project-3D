#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "LightSource.h"
#include "Camera.h"
//#include "Frustum.h"

using namespace DirectX;

///////////////////
// Defines       //
///////////////////

// window size
#define W_WIDTH 1280.0f
#define W_HEIGHT 720.0f

//Fov (radians)
#define FOV 0.45f//0.45f

// PROJECTION RANGE
#define PROJECTION_NEAR_Z 0.1f
#define PROJECTION_FAR_Z 500.0f

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
	XMVECTOR Specular;
};

// resource storing camera source
struct CameraMatrix {
	XMVECTOR Origin;
	XMVECTOR Focus;
};

// a resource to store world matrix for objects in the GPU
struct ObjectW {
	XMMATRIX World;
};

// a resource to store type, origin and view,porojection matrix for lights in the GPU
struct LightVP {
	UINT LightType;
	XMFLOAT3 Origin;
	XMMATRIX ViewProjection;
	XMMATRIX RotatedViewProjection[5];
};

// a resource to store world,view,porojection matrix for object in the GPU
struct CameraWVP {
	XMMATRIX World;
	XMMATRIX ViewProjection;
};

// basic vertex struct µ
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
	int ambient;
	XMVECTOR position;
	XMVECTOR direction;
	XMVECTOR colour;
	float intensity;
	float lightFocus;
};

struct WorldSpace {
	float rotation_x;
	float rotation_y;
	float rotation_z;
	float offset_x;
	float offset_y;
	float offset_z;
	float scale_x;
	float scale_y;
	float scale_z;
};

/////////////////////
// Variables       //
/////////////////////

// viewport
extern D3D11_VIEWPORT* vp;

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

//blend resource
extern ID3D11BlendState* gBlendStateLight;

// a resource to store Vertices in the GPU
extern int gnrOfVert[5];
extern ID3D11Buffer*ppVertexBuffers[5];
extern XMFLOAT3 ObjectReflection[5];
extern ID3D11Buffer *heightmapBuffer;
extern int nrOfHMVert;
extern int nrOfVertexBuffers;
//extern char* textureName;

extern ID3D11Buffer* gDeferredQuadBuffer;

// input layouts
extern ID3D11InputLayout* gShadowVertexLayout;
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
extern ID3D11VertexShader* gShadowVertexShader;
extern ID3D11GeometryShader* gShadowGeometryShader;
extern ID3D11PixelShader* gShadowPixelShader;
extern ID3D11VertexShader* gVertexShader;
extern ID3D11GeometryShader* gGeometryShader;
extern ID3D11PixelShader* gPixelShader;
extern ID3D11PixelShader* gBlendShader;
extern ID3D11ComputeShader* gComputeShader;
extern ID3D11VertexShader* gLightVertexShader;
extern ID3D11PixelShader* gLightPixelShader;

extern AmbientSpecular* gAmbientSpecularData;
extern ID3D11Buffer* gAmbientSpecularBuffer;

extern CameraMatrix* gCameraMatrix;
extern ID3D11Buffer* gCameraMatrixBuffer;

extern CameraWVP* gWorldMatrix;
extern ID3D11Buffer* gWorldMatrixBuffer;

extern ObjectW* gObjectMatrix;
extern ID3D11Buffer* gObjectMatrixBuffer;

extern LightVP* gLightMatrix;
extern ID3D11Buffer* gLightMatrixBuffer;

extern ID3D11Buffer* gLightDataBuffer;

extern LightSource* Lights;
extern int nrOfLights;

//World/View/Projection
extern XMMATRIX World;
extern XMMATRIX View;
extern XMMATRIX ViewRotated[5];
extern XMMATRIX Projection;

// CAMERAclass
extern Camera camera;

// FrustumOfCamera

//extern Frustum frustumCamera;

// keeping track of current rotation
extern float rotation;

extern int nrOfVertices;
extern Heightmap g_heightmap;
extern TriangleVertex* g_map;

extern int gnrOfVertices;

//clear pointers
extern ID3D11ShaderResourceView* nullSRV[1];
extern ID3D11Buffer* nullCB;

extern ID3D11ShaderResourceView* gTextureSRV[5];
extern ID3D11Resource* gTexture2D[5];

extern ID3D11ShaderResourceView* gMapTexturesSRV[4];
extern ID3D11Resource* gMapTextureResource[4];