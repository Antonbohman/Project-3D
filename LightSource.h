#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

#define S_WIDTH 640.0f
#define S_HEIGHT 480.0f

#define SHADOW_MAPS 6

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

	ID3D11Texture2D* shadowRenderTargetTexture[6];
	ID3D11RenderTargetView* shadowRenderTargetView[6];
	ID3D11ShaderResourceView* shadowShaderResourceView[6];

public:
	LightSource(const int type = 0, const int ambient = 0, const XMVECTOR position = XMVectorSet(0.0f,0.0f,0.0f,0.0f),
				const XMVECTOR direction = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
				const XMVECTOR colour = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
				const float intensity = 0.0f, const float lightFocus = 0.0f);
	LightSource(const LightSource& origObj);
	LightSource& operator=(const LightSource& origObj);
	virtual ~LightSource();

	HRESULT createShadowBuffer(ID3D11Device* device);

	HRESULT Prepare(ID3D11DeviceContext* deviceContext, int index) const;
	HRESULT Load(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pBuffer) const;

	int LightType() const;
	ID3D11ShaderResourceView* ShadowMap(int index) const;

	XMMATRIX getView() const;
	XMMATRIX getProjection() const;
};

