#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

//shadow map render size
#define MUL 24
#define S_WIDTH (640.0f*MUL)
#define S_HEIGHT (480.0f*MUL)

#define C_MUL 6
#define S_C_WIDTH (1280.0f*C_MUL)
#define S_C_HEIGHT (720.0f*C_MUL)

#define DIR_MUL 6
#define S_DIR_WIDTH (1000.0*DIR_MUL)
#define S_DIR_HEIGHT (1000.0f*DIR_MUL)

// light types
#define L_POINT 0
#define L_SPOT 1
#define L_DIRECTIONAL 2

class LightSource {
private:
	struct LightData {
		UINT type;
		UINT ambient;
		float intensity;
		float lightFocus;
		XMVECTOR position;
		XMVECTOR direction;
		XMVECTOR colour;
		XMMATRIX viewProjection;
		XMMATRIX rotatedViewProjection[5];
	};

	LightData data;

	D3D11_VIEWPORT vp;

	XMMATRIX views[6];
	XMMATRIX projection;

	ID3D11Texture2D* shadowRenderTargetTexture;
	ID3D11DepthStencilView* shadowDepthStencilView;
	ID3D11ShaderResourceView* shadowShaderResourceView;

	void setViewport();
	void setViews();
	void setProjection();
	void setViewProjection();

public:
	LightSource(const int type = 0, const int ambient = 0,
				const XMVECTOR position = XMVectorSet(0.0f,0.0f,0.0f,0.0f),
				const XMVECTOR direction = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f),
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
    XMMATRIX getViewProjection(int index) const;
	ID3D11ShaderResourceView* getShadowMap() const;

	//void updateRenderPoint();
};

