#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

//shadow map render size
#define S_WIDTH 640.0f/2
#define S_HEIGHT 480.0f/2

// light types
#define L_POINT 0
#define L_SPOT 1
#define L_DIRECTIONAL 2

class LightSource {
private:
	struct LightData {
		int type;
		int ambient;
		XMVECTOR position;
		XMVECTOR direction;
		XMVECTOR colour;
		float intensity;
		float lightFocus;
	};

	LightData data;

	ID3D11Texture2D* shadowRenderTargetTexture;
	ID3D11DepthStencilView* shadowDepthStencilView;
	ID3D11ShaderResourceView* shadowShaderResourceView;

	D3D11_VIEWPORT vp;

	void createViewport();

public:
	LightSource(const int type = 0, const int ambient = 0, const XMVECTOR position = XMVectorSet(0.0f,0.0f,0.0f,0.0f),
				const XMVECTOR direction = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
				const XMVECTOR colour = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
				const float intensity = 0.0f, const float lightFocus = 0.0f);
	LightSource(const LightSource& origObj);
	LightSource& operator=(const LightSource& origObj);
	virtual ~LightSource();

	HRESULT createShadowBuffer(ID3D11Device* device);

	void prepareShadowRender(ID3D11DeviceContext* deviceContext) const;
	HRESULT loadShadowBuffers(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pBuffer) const;
	HRESULT loadLightBuffers(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pBuffer) const;

	int getLightType() const;
	XMVECTOR getOrigin() const;
	ID3D11ShaderResourceView* getShadowMap() const;

	XMMATRIX getView(int index) const;
	XMMATRIX getProjection() const;
};

