#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

class LightSource {
private:
	struct LightData {
		int type;
		int ambient;
		XMVECTOR position;
		XMVECTOR direction;
		XMVECTOR colour;
		float intensity;
		float angleX; float angleY;
	};

	LightData data;

public:
	LightSource(const int type = 0, const int ambient = 0, const XMVECTOR position = XMVectorSet(0.0f,0.0f,0.0f,0.0f),
				const XMVECTOR direction = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
				const XMVECTOR colour = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
				const float intensity = 0.0f, const float angleX = 0.0f, const float angleY = 0.0f);
	LightSource(const LightSource& origObj);
	LightSource& operator=(const LightSource& origObj);
	virtual ~LightSource();

	bool Load(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pBuffer) const;
};

