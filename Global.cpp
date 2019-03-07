#include "Global.h"

// Most directX Objects are COM Interfaces
// https://es.wikipedia.org/wiki/Component_Object_Model
IDXGISwapChain* gSwapChain = nullptr;

// Device and DeviceContext are the most common objects to
// instruct the API what to do. It is handy to have a reference
// to them available for simple applications.
ID3D11Device* gDevice = nullptr;
ID3D11DeviceContext* gDeviceContext = nullptr;

// sampling resource
ID3D11SamplerState* gSampling = nullptr;

// a resource to store Vertices in the GPU
//ID3D11Buffer* gVertexBufferMap = nullptr;
//ID3D11Buffer* gVertexBufferObj = nullptr;
//TriangleVertex* gMap = nullptr;
//TriangleVertex* gObject = nullptr;
int gnrOfVert[5];
ID3D11Buffer *ppVertexBuffers[5];
ID3D11Buffer *heightmapBuffer;
int nrOfHMVert;
int nrOfVertexBuffers;
//int gTotalNrOfVert = 0;
char* textureName = nullptr;

ID3D11Buffer* gDeferredQuadBuffer = nullptr;

// input layouts
ID3D11InputLayout* gShadowVertexLayout = nullptr;
ID3D11InputLayout* gVertexLayout = nullptr;
ID3D11InputLayout* gLightVertexLayout = nullptr;

// render targets
ID3D11RenderTargetView* gBackbufferRTV = nullptr;
ID3D11Texture2D* gRenderTargetTextureArray[G_BUFFER];
ID3D11RenderTargetView* gRenderTargetViewArray[G_BUFFER];
ID3D11ShaderResourceView* gShaderResourceViewArray[G_BUFFER];

// resources for depth buffer image
ID3D11DepthStencilView* gDepth = nullptr;

// resources that represent shaders
ID3D11VertexShader* gShadowVertexShader = nullptr;
ID3D11VertexShader* gVertexShader = nullptr;
ID3D11GeometryShader* gGeometryShader = nullptr;
ID3D11PixelShader* gPixelShader = nullptr;
ID3D11PixelShader* gBlendShader = nullptr;
ID3D11ComputeShader* gComputeShader = nullptr;
ID3D11VertexShader* gLightVertexShader = nullptr;
ID3D11PixelShader* gLightPixelShader = nullptr;

AmbientSpecular* gAmbientSpecularData = nullptr;
ID3D11Buffer* gAmbientSpecularBuffer = nullptr;

CameraMatrix* gCameraMatrix = nullptr;
ID3D11Buffer* gCameraMatrixBuffer = nullptr;

CameraWVP* gWorldMatrix = nullptr;
ID3D11Buffer* gWorldMatrixBuffer = nullptr;

LightWVP* gLightMatrix = nullptr;
ID3D11Buffer* gLightMatrixBuffer = nullptr;

ID3D11Buffer* gLightDataBuffer = nullptr;

LightSource* Lights = nullptr;
int nrOfLights = 0;

// CAMERAVIEW
Camera camera({ 0.0f,10.0f,-20.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f });

//World/View/Projection
XMMATRIX World;
XMMATRIX View;
XMMATRIX Projection;

// keeping track of current rotation
float rotation = 1.5f*XM_PI;

int nrOfVertices = 0;
Heightmap g_heightmap;
TriangleVertex* g_map;

int gnrOfVertices = 0;

//clear pointers
ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
ID3D11Buffer* nullCB = nullptr;

ID3D11ShaderResourceView* gTextureSRV[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
ID3D11Resource* gTexture2D[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };

ID3D11ShaderResourceView* gMapTexturesSRV[4] = { nullptr, nullptr, nullptr, nullptr };
ID3D11Resource* gMapTextureResource[4] = { nullptr, nullptr, nullptr, nullptr };