#include <windows.h>
#include <chrono>

//#include "bth_image.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

//window size
#define W_WIDTH 640.0f
#define W_HEIGHT 480.0f

//define number of vertices used in rendering
#define VERTICES 6

using namespace DirectX;
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

//CAMERAVIEW
XMVECTOR CameraView = { 0.0f, 0.0f, -2.0f, 0.0f };

// resource storing lightning source
struct LightData {
	XMVECTOR ambient;
	XMVECTOR light;//POSITION
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


//keeping track of current rotation
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

void CreateTriangleData() {
	// Array of Structs (AoS)
	TriangleVertex triangleVertices[VERTICES] =
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
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// how big in bytes each element in the buffer is.
	bufferDesc.ByteWidth = sizeof(triangleVertices);

	// this struct is created just to set a pointer to the
	// data containing the vertices.
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;

	// create a Vertex Buffer
	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
}

void CreateConstantBuffer() {
	//allocate space in memory aligned to a multitude of 16
	gLightData = (LightData*)_aligned_malloc(sizeof(LightData), 16);

	//set our faked light source values, 
	//since we won't be updating these values while program is running
	gLightData->ambient = XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f);
	gLightData->light = XMVectorSet(0.0f, 0.0f, -7.0f, 1.0f);
	gLightData->colour = XMVectorSet(1.0f, 1.0f, 1.0f, 10.0f);
	gLightData->cameraView = CameraView;

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

bool loadHeightMap(char* filename, Heightmap &heightmap)
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
	imageSize = heightmap.imageHeight * heightmap.imageWidth * 3; //3 is for the three values RGB

	//array of image data
	unsigned char* bitmapImage = new unsigned char[imageSize];

	//Find start of iamge data
	fseek(fileptr, bitmapFileHeader.bfOffBits, SEEK_SET);

	//read data into bitmapimage
	fread(bitmapImage, 1, imageSize, fileptr);

	//close file
	fclose(fileptr);

	//array of vertice positions
	heightmap.verticesPos = new XMFLOAT3[heightmap.imageHeight * heightmap.imageWidth];

	int counter = 0; //Eftersom bilden är i gråskala så är alla värden RGB samma värde, därför läser vi bara R

	//float heightFactor = 10.0f; //mountain smoothing

	//read and put vertex position
	for (int i = 0; i < heightmap.imageHeight; i++)
	{
		for (int j = 0; j < heightmap.imageWidth; j++)
		{
			height = bitmapImage[counter];
			index = (heightmap.imageHeight * i) + j;

			heightmap.verticesPos[index].x = (float)j;
			heightmap.verticesPos[index].y = (float)height/*/heightFactor*/;
			heightmap.verticesPos[index].z = (float)i;

			counter += 3;
		}
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
	gDeviceContext->Draw(VERTICES, 0);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	//create two timestamps variables and a delta between them to adjust update frequency
	time_point<high_resolution_clock>start = high_resolution_clock::now();
	time_point<high_resolution_clock>end = high_resolution_clock::now();
	duration<double, std::ratio<1, 8>> delta;

	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance); //1. Skapa fönster

	if (wndHandle) {
		CreateDirect3DContext(wndHandle); //2. Skapa och koppla SwapChain, Device och Device Context

		SetViewport(); //3. Sätt viewport

		CreateShaders(); //4. Skapa vertex- och pixel-shaders

		CreateTriangleData(); //5. Definiera triangelvertiser, 6. Skapa vertex buffer, 7. Skapa input layout

		CreateConstantBuffer(); //8. Create constant buffers

		//CreateTextureResource(); //9. Create and store texture image

		Heightmap _heightmap;

		if (!loadHeightMap("terrain.bmp", _heightmap)) return 404;

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

				//upate rotation depending on time since last update
				rotation += delta.count()*0.05f;

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
					 CameraView,
					{ 0.0f, 0.0f, 0.0f, 0.0f },
					{ 0.0f, 1.0f, 0.0f, 0.0f }
				);
				View = XMMatrixTranspose(View);

				XMMATRIX Projection = XMMatrixPerspectiveFovLH(
					(float)XM_PI*0.45,
					(float)W_WIDTH / (float)W_HEIGHT,
					0.1f,
					20.0f
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