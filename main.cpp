/*SAFE VERSION! 2019-03-12 17:16 Filip*/
#pragma once
#include <windows.h>
#include <chrono>
#include <algorithm>
#include <Mouse.h>
#include <Keyboard.h>

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
#include "ConstantBuffers.h"
#include "Shaders.h"
#include "Loader.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace std::chrono;

std::unique_ptr<DirectX::Mouse>m_mouse;
std::unique_ptr<DirectX::Keyboard>m_keyboard;

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HRESULT CreateDirect3DContext(HWND wndHandle);

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
	nrOfLights = 3;
	Lights = new LightSource[nrOfLights];

	Lights[0] = LightSource(L_SPOT, 5, XMVectorSet(-20.0f, 20.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 20.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f), 200.0f, 100.0f);
	Lights[1] = LightSource(L_SPOT, 5, XMVectorSet(-50.0f, 50.0f, 0.0f, 0.0f), XMVectorSet(-50.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f), 200.0f, 100.0f);
	Lights[2] = LightSource(L_SPOT, 5, XMVectorSet(230.0f, 11.0f, 238.0f, 0.0f), XMVectorSet(247.0f, 60.0f, 240.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f), 200.0f, 100.0f);

	for (int i = 0; i < nrOfLights; i++) {
		Lights[i].createShadowBuffer(gDevice);
	}
}

void createBlendState() {
	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
	blendStateDesc.AlphaToCoverageEnable = false;
	blendStateDesc.IndependentBlendEnable = false;
	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	HRESULT res = gDevice->CreateBlendState(&blendStateDesc, &gBlendStateLight);
}

void createViewport() {
	svp[0].Width = (float)W_WIDTH / 2;
	svp[0].Height = (float)W_HEIGHT / 2;
	svp[0].MinDepth = 0.0f;
	svp[0].MaxDepth = 1.0f;
	svp[0].TopLeftX = 0;
	svp[0].TopLeftY = 0;

	svp[1].Width = (float)W_WIDTH / 2;
	svp[1].Height = (float)W_HEIGHT / 2;
	svp[1].MinDepth = 0.0f;
	svp[1].MaxDepth = 1.0f;
	svp[1].TopLeftX = (float)W_WIDTH / 2;
	svp[1].TopLeftY = 0;

	svp[2].Width = (float)W_WIDTH / 2;
	svp[2].Height = (float)W_HEIGHT / 2;
	svp[2].MinDepth = 0.0f;
	svp[2].MaxDepth = 1.0f;
	svp[2].TopLeftX = 0;
	svp[2].TopLeftY = (float)W_HEIGHT / 2;;

	svp[3].Width = (float)W_WIDTH / 2;
	svp[3].Height = (float)W_HEIGHT / 2;
	svp[3].MinDepth = 0.0f;
	svp[3].MaxDepth = 1.0f;
	svp[3].TopLeftX = (float)W_WIDTH / 2;
	svp[3].TopLeftY = (float)W_HEIGHT / 2;;

	if (!vp) {
		vp = new D3D11_VIEWPORT;
		vp->Width = (float)W_WIDTH;
		vp->Height = (float)W_HEIGHT;
		vp->MinDepth = 0.0f;
		vp->MaxDepth = 1.0f;
		vp->TopLeftX = 0;
		vp->TopLeftY = 0;
	}
}

void SetViewport(bool forceSingle) {
	if (renderOpt & RENDER_DOUBLE_VIEWPORT && !forceSingle) {
		gDeviceContext->RSSetViewports(4, svp);
	} else {
		gDeviceContext->RSSetViewports(1, vp);
	}
}

void CreateObjects() {
	loadHeightMap("Objects/Heightmaps/halv_island.bmp");

	loadMultiTextureMap("Objects/Heightmaps/halv_islandMT.bmp");


	LoadObjectFile("Objects/OBJs/fish.obj", XMINT3(0, 20, 0));


	LoadObjectFile("Objects/OBJs/Mars.obj", XMINT3(5, 25, 5));


	LoadObjectFile("Objects/OBJs/Moon.obj", XMINT3(0, 25, -5));


	LoadObjectFile("Objects/OBJs/trex.obj", XMINT3(460, -240, 95));
}

void RenderWireframe() {
	// clear the back buffer to a black
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	setWireframeShaders();

	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);
	
	// make sure our depth buffer is cleared to black each time we render
	gDeviceContext->ClearDepthStencilView(gDepth, D3D11_CLEAR_DEPTH, 1.0f, 0);

	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorldMatrixBuffer);
	gDeviceContext->GSSetConstantBuffers(0, 1, &gCameraMatrixBuffer);

	updateCameraValues();
	setCameraViewProjectionSpace();

	//set world space for height map and update wvp matrix
	//set specular for height map
	setWorldSpace({ 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f });
	updateCameraWorldViewProjection();

	//Render heightmap
	setVertexBuffer(heightmapBuffer, sizeof(TriangleVertex), 0);
	gDeviceContext->Draw(nrOfHMVert, 0);

	for (int i = 0; i < nrOfVertexBuffers; i++) {
		setWorldSpace({ 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f });
		updateCameraWorldViewProjection();

		//Render objects
		setVertexBuffer(ppVertexBuffers[i], sizeof(TriangleVertex), 0);
		gDeviceContext->Draw(gnrOfVert[i], 0);
	}
	
	gDeviceContext->VSSetConstantBuffers(0, 1, &nullCB);
	gDeviceContext->GSSetConstantBuffers(0, 1, &nullCB);
}

void RenderShadowMaps() {
	setShadowShaders();

	gDeviceContext->VSSetConstantBuffers(0, 1, &gObjectMatrixBuffer);
	gDeviceContext->GSSetConstantBuffers(0, 1, &gLightMatrixBuffer);

	for (int i = 0; i < nrOfLights; i++) {
		Lights[i].prepareShadowRender(gDeviceContext);

		setLightViewProjectionSpace(&Lights[i]);

		//set world space for height map and update wvp
		setWorldSpace({ 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f });
		updateObjectWorldSpace();
		updateLightViewProjection(&Lights[i]);

		//Render heightmap
		setVertexBuffer(heightmapBuffer, sizeof(TriangleVertex), 0);
		gDeviceContext->Draw(nrOfHMVert, 0);

		for (int i = 0; i < nrOfVertexBuffers; i++) {
			//set world space for each object and update wvp
			setWorldSpace({ 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f });
			updateObjectWorldSpace();
			updateLightViewProjection(&Lights[i]);

			//Render objects
			setVertexBuffer(ppVertexBuffers[i], sizeof(TriangleVertex), 0);
			gDeviceContext->Draw(gnrOfVert[i], 0);
		}
	}

	gDeviceContext->VSSetConstantBuffers(0, 1, &nullCB);
	gDeviceContext->GSSetConstantBuffers(0, 1, &nullCB);
}

void RenderBuffers(float notToRender) {
	// clear the back buffer to a deep blue
	float clearColor[] = { 0.45f, 0.95f, 1.0f, 1.0f };

	updateCameraValues();
	setCameraViewProjectionSpace();

	// Clear the render target buffers.
	for (int i = 0; i < G_BUFFER; i++) {
		gDeviceContext->ClearRenderTargetView(gRenderTargetViewArray[i], clearColor);
	}

	// make sure our depth buffer is cleared to black each time we render
	gDeviceContext->ClearDepthStencilView(gDepth, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Set the sampler state in the pixel shader.
	gDeviceContext->PSSetSamplers(0, 1, &gSampling);

	//bind our constant buffers to coresponding shader
	gDeviceContext->VSSetConstantBuffers(0, 1, &gWorldMatrixBuffer);
	gDeviceContext->GSSetConstantBuffers(0, 1, &gCameraMatrixBuffer);
	gDeviceContext->PSSetConstantBuffers(0, 1, &gAmbientSpecularBuffer);

	//Set heightmap shader options
	SetBlendShaders();

	//load map textures
	for (int i = 0; i < 4; i++) {
		gDeviceContext->PSSetShaderResources(i, 1, &gMapTexturesSRV[i]);
	}

	//set world space for height map and update wvp matrix
	//set specular for height map

	setWorldSpace({ 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f });
	updateCameraWorldViewProjection();
	setSpecularValues(XMVectorSet(1, 1, 1, 1));

	//Render heightmap
	setVertexBuffer(heightmapBuffer, sizeof(TriangleVertex), 0);
	gDeviceContext->Draw(nrOfHMVert, 0);

	//Release
	for (int i = 0; i < 4; i++) {
		gDeviceContext->PSSetShaderResources(i, 1, &nullSRV[0]);
	}

	//Set object shader options
	SetDeferredShaders();

	WorldSpace copies[2];
	copies[0].offset_x = 0; copies[0].offset_y = 30; copies[0].offset_z = 10;

	copies[1].offset_x = 0; copies[1].offset_y = 40; copies[1].offset_z = 10;

	//Render objects!
	for (int i = 0; i < nrOfVertexBuffers; i++) {
	
		

		if (0 == i )
		{
			setWorldSpace({ 0.0f,0.0f,0.0f,copies[0].offset_x,copies[0].offset_y,copies[0].offset_z,3.0f,3.0f,3.0f });
			
			updateCameraWorldViewProjection();
			setSpecularValues(XMVectorSet(1, 1, 1, 1000));

			//set object texture
			gDeviceContext->PSSetShaderResources(0, 1, &gTextureSRV[i]);

			//Render objects
			setVertexBuffer(ppVertexBuffers[i], sizeof(TriangleVertex), 0);
			gDeviceContext->Draw(gnrOfVert[i], 0);

			setWorldSpace({ 0.0f,0.0f,0.0f,copies[1].offset_x,copies[1].offset_y,copies[1].offset_z,1.0f,1.0f,1.0f });
			updateCameraWorldViewProjection();
			//Render objects
			gDeviceContext->Draw(gnrOfVert[i], 0);
			
		}
		else
		{
			//set world space for object and update wvp matrix
			//set specular for object
			setWorldSpace({ 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f });
			updateCameraWorldViewProjection();
			setSpecularValues(XMVectorSet(1, 1, 1, 1000));

			//set object texture
			gDeviceContext->PSSetShaderResources(0, 1, &gTextureSRV[i]);

			//Render objects
			setVertexBuffer(ppVertexBuffers[i], sizeof(TriangleVertex), 0);
			gDeviceContext->Draw(gnrOfVert[i], 0);
		}
	}

	//Release
	for (int i = 0; i < nrOfVertexBuffers; i++) {
		gDeviceContext->PSSetShaderResources(i, 1, &nullSRV[0]);
	}

	gDeviceContext->VSSetConstantBuffers(0, 1, &nullCB);
	gDeviceContext->GSSetConstantBuffers(0, 1, &nullCB);
	gDeviceContext->PSSetConstantBuffers(0, 1, &nullCB);
}

void RenderLights() {
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//set shaders for light calculation (final pass)
	SetLightShaders();

	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);

	//set our quad as fragment to render
	setVertexBuffer(gDeferredQuadBuffer, sizeof(PositionVertex), 0);

	//bind our constant buffers to shaders
	gDeviceContext->PSSetConstantBuffers(0, 1, &gCameraMatrixBuffer);
	gDeviceContext->PSSetConstantBuffers(1, 1, &gLightDataBuffer);

	//bind our g-buffer textures to pixelshader
	gDeviceContext->PSSetShaderResources(0, 1, &gShaderResourceViewArray[0]);
	gDeviceContext->PSSetShaderResources(1, 1, &gShaderResourceViewArray[1]);
	gDeviceContext->PSSetShaderResources(2, 1, &gShaderResourceViewArray[2]);
	gDeviceContext->PSSetShaderResources(3, 1, &gShaderResourceViewArray[3]);

	// render each light source
	for (int i = 0; i < nrOfLights; i++) {
		Lights[i].loadLightBuffers(gDeviceContext, gLightDataBuffer);
		gDeviceContext->Draw(6, 0);
	}

	//Release
	gDeviceContext->PSSetShaderResources(0, 1, &nullSRV[0]);
	gDeviceContext->PSSetShaderResources(1, 1, &nullSRV[0]);
	gDeviceContext->PSSetShaderResources(2, 1, &nullSRV[0]);
	gDeviceContext->PSSetShaderResources(3, 1, &nullSRV[0]);

	gDeviceContext->PSSetConstantBuffers(0, 1, &nullCB);
	gDeviceContext->PSSetConstantBuffers(1, 1, &nullCB);

	//gDeviceContext->CopyResource(gBlurShaderResource);

	//pBackBuffer

	gDeviceContext->CSSetShaderResources(0, 1, &gBlurShaderResource);
	gDeviceContext->Dispatch(45, 45, 1);
}

void updateKeyAndMouseInput(Frustum* camFrustum, duration<double, std::ratio<1, 15>> delta) 
{
	bool freeFlight = renderOpt & RENDER_FREE_FLIGHT;
	bool culling = renderOpt & RENDER_CULLING;

	//float DontRender[6] = {-1};
	float DontRender = -1;
	//FREE FLIGHT WITH O key
	//HORIZONTAL movement with P 

	/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>MOVEMENT<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
	{//IN FREEFLIGHT				O key
	 //INTO CAMERA FORWARD		W
	 //INTO CAMERA BACKWARDS		S
	 //CAMERA RIGHT				D
	 //CAMERA LEFT				A
	 //CAMERA UP					Q
	 //CAMERA DOWN				E

	 //IN HORIZONTAL				P key
	 //CAMERA FORWARD			W
	 //CAMERA BACKWARDS			S
	 //CAMERA RIGHT				D
	 //CAMERA LEFT				A

	 //OTHER
	 //NV corner				F1				
	 //NE corner				F2
	 //SE corner				F3
	 //SV corner				F4
	}
	/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>MOVEMENT<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

	//KEYBOARD 
	{
		auto kb = m_keyboard->GetState();
		//SHORT COMMANDS
		{
			if (kb.Escape) {
				camera.SetCamPos(camera.GetCamOriginalPos());
				camera.SetYawAndPitch(0, 0);
				camera.SetCamTarget(camera.GetCamOriginalTarget());
			}
			if (kb.F1) {
				camera.SetCamPos({ float(-g_heightmap.imageWidth / 2),3.0f,float(+g_heightmap.imageHeight / 2),0.0f });

				float hyp = sqrt((pow(g_heightmap.imageWidth / 2, 2) + pow(g_heightmap.imageHeight / 2, 2)));

				float angel = ((g_heightmap.imageWidth / 2) / hyp);

				camera.SetYawAndPitch(XM_PI*angel, 0);
			}
			if (kb.F2) {
				camera.SetCamPos({ float(+g_heightmap.imageWidth / 2),3.0f,float(+g_heightmap.imageHeight / 2),0.0f });

				float hyp = sqrt((pow(g_heightmap.imageWidth / 2, 2) + pow(g_heightmap.imageHeight / 2, 2)));

				float angel = ((g_heightmap.imageWidth / 2) / hyp) + 0.5;
				//angel = 0.0f;
				camera.SetYawAndPitch(XM_PI*angel, 0);
			}
			if (kb.F3) {
				camera.SetCamPos({ float(+g_heightmap.imageWidth / 2),3.0f,float(-g_heightmap.imageHeight / 2),0.0f });

				float hyp = sqrt((pow(g_heightmap.imageWidth / 2, 2) + pow(g_heightmap.imageHeight / 2, 2)));

				float angel = ((g_heightmap.imageWidth / 2) / hyp) + 1.0;

				camera.SetYawAndPitch(XM_PI*angel, 0);
			}
			if (kb.F4) {
				camera.SetCamPos({ float(-g_heightmap.imageWidth / 2),3.0f,float(-g_heightmap.imageHeight / 2),0.0f });

				float hyp = sqrt((pow(g_heightmap.imageWidth / 2, 2) + pow(g_heightmap.imageHeight / 2, 2)));

				float angel = ((g_heightmap.imageWidth / 2) / hyp) + 1.5;

				camera.SetYawAndPitch(XM_PI*angel, 0);
			}
			if (kb.O) {
				freeFlight = true;
			}
			if (kb.P) {
				freeFlight = false;
			}
		}

		Vector3 moveInDepthCameraClass = Vector3::Zero;
		Vector3 deltaChange = Vector3::Zero;
		bool run = false;

		//UPDATE VECTOR

		if (kb.LeftShift) {
			run = true;
		}

		//pineapple in a green pie
		if (kb.W) {//FORWARD IN

			if (freeFlight) {
				moveInDepthCameraClass += camera.GetCameraNormal();
			} else {
				moveInDepthCameraClass += camera.GetCamForward();
			}

		}
		if (kb.S) { //BACK
			if (freeFlight) {
				moveInDepthCameraClass -= camera.GetCameraNormal();
			} else {
				moveInDepthCameraClass -= camera.GetCamForward();
			}

		}
		if (kb.D) { //RIGHT
			deltaChange.x += 1.0f;

		}
		if (kb.A) { //LEFT
			deltaChange.x -= 1.0f;
		}
		if (kb.Q) { //UP
			if (freeFlight)
				deltaChange.y += 1.0f;
		}
		if (kb.E) { //DOWN
			if (freeFlight)
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
			camera.UpdateCamera({ deltaChange.x,deltaChange.y,deltaChange.z }, run, float(delta.count()));

			//Walking on terrain
			if (!freeFlight) {
				Vector4 CameraPos = camera.GetCamPos();

				//TEST byte ordningen så det liknar en graf.

				XMINT2 nRoundedPos;
				nRoundedPos.x = CameraPos.x;
				nRoundedPos.y = CameraPos.z;

				//avrundar till närmsta heltal
				if (nRoundedPos.x < 0) {
					if (float(nRoundedPos.x) - 0.5f > CameraPos.x) nRoundedPos.x--;
				} else {
					if (float(nRoundedPos.x) + 0.5f < CameraPos.x) nRoundedPos.x++;
				}
				if (nRoundedPos.y < 0) {
					if (float(nRoundedPos.y) - 0.5f > CameraPos.z) nRoundedPos.y--;
				} else {
					if (float(nRoundedPos.y) + 0.5f < CameraPos.z) nRoundedPos.y++;
				}

				nRoundedPos.x += g_heightmap.imageWidth / 2;
				nRoundedPos.y += g_heightmap.imageHeight / 2;

				//Avrundar så ingen ogiltig arrayplats nåss
				if (nRoundedPos.x < 0) nRoundedPos.x = 0;
				if (nRoundedPos.x >= g_heightmap.imageWidth) nRoundedPos.x = g_heightmap.imageWidth - 1;

				if (nRoundedPos.y < 0) nRoundedPos.y = 0;
				if (nRoundedPos.y >= g_heightmap.imageHeight) nRoundedPos.y = g_heightmap.imageHeight - 1;


				int index = (nRoundedPos.y * g_heightmap.imageWidth) + nRoundedPos.x;
				float newHeight = (g_heightmap.verticesPos[index].y + WALKING_HEIGHT);

				//ställer pitch, yaw, ny höjd 
				camera.SetCameraHeight(newHeight);

				//camFrustum->calculateFrustum(FOV,W_WIDTH,W_HEIGHT);

				//if(camFrustum->pointInFrustum({ 5.0f, 25.0f, 5.0f, 0.0f })!=0 )
				//{
				//	//LOOKING AT MARS REMOVES FISH
				//	DontRender = 0;
				//}
			}

		}
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//create two timestamps variables and a delta between them to adjust update frequency
	time_point<high_resolution_clock>start = high_resolution_clock::now();
	time_point<high_resolution_clock>end = high_resolution_clock::now();
	duration<double, std::ratio<1, 15>> delta;

	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance);

	Frustum camFrustum(&camera);

	//Mouse and keyboard ini (ONLY MOUSE)
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(wndHandle);

	if (wndHandle)
	{
		CreateDirect3DContext(wndHandle);

		CreateShaders();

		createBlendState();

		createViewport();

		CreateDeferredQuad();

		CreateConstantBuffer();

		CreateSampling();

		CreateLigths();

		CreateObjects();

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

				updateKeyAndMouseInput(&camFrustum, delta);

				if (renderOpt & RENDER_WIREFRAME) {
					SetViewport(false);

					RenderWireframe();
				} else {
					RenderShadowMaps();

					SetViewport(false);

					RenderBuffers(0);

					SetViewport(true);

					RenderLights();
				}

				gSwapChain->Present(0, 0); //11. Växla front- och back-buffer
			}
		}

		//destroy
		DestroyShaders();
		DestroyGlobals();
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
	//gDevice->CreateShaderResourceView(pBackBuffer,,&gBlurShaderResource) // Nevermind this!
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

		if (renderOpt & RENDER_WIREFRAME) {
			D3D11_RASTERIZER_DESC rastDesc;
			ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));

			rastDesc.FillMode = D3D11_FILL_WIREFRAME;
			rastDesc.CullMode = D3D11_CULL_BACK;
			rastDesc.FrontCounterClockwise = false;
			rastDesc.DepthBias = 0;
			rastDesc.DepthBiasClamp = 0.0f;
			rastDesc.SlopeScaledDepthBias = 0.0f;
			rastDesc.DepthClipEnable = true;
			rastDesc.ScissorEnable = false;
			rastDesc.MultisampleEnable = false;
			rastDesc.AntialiasedLineEnable = false;

			ID3D11RasterizerState* gRasterizerState;

			hr = gDevice->CreateRasterizerState(
				&rastDesc,
				&gRasterizerState
			);

			gDeviceContext->RSSetState(gRasterizerState);
		}

	}

	return hr;
}
