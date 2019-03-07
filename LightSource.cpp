#include "LightSource.h"

LightSource::LightSource(const int type, const int ambient, const XMVECTOR position,
						const XMVECTOR direction, const XMVECTOR colour, 
						const float intensity, const float lightFocus) {
	data.type = type;
	data.ambient = ambient;
	data.position = position;
	data.direction = direction;
	data.colour = colour;
	data.intensity = intensity;
	data.lightFocus = lightFocus;

	for (int i = 0; i < SHADOW_MAPS; i++) {
		shadowRenderTargetTexture[i] = nullptr;
		shadowRenderTargetView[i] = nullptr;
		shadowShaderResourceView[i] = nullptr;
	}
}

LightSource::LightSource(const LightSource& origObj) {
	this->data.type = origObj.data.type;
	this->data.ambient = origObj.data.ambient;
	this->data.position = origObj.data.position;
	this->data.direction = origObj.data.direction;
	this->data.colour = origObj.data.colour;
	this->data.intensity = origObj.data.intensity;
	this->data.lightFocus = origObj.data.lightFocus;

	for (int i = 0; i < SHADOW_MAPS; i++) {
		shadowRenderTargetTexture[i] = nullptr;
		shadowRenderTargetView[i] = nullptr;
		shadowShaderResourceView[i] = nullptr;
	}
}

LightSource& LightSource::operator=(const LightSource& origObj) {
	if (this != &origObj) {
		this->data.type = origObj.data.type;
		this->data.ambient = origObj.data.ambient;
		this->data.position = origObj.data.position;
		this->data.direction = origObj.data.direction;
		this->data.colour = origObj.data.colour;
		this->data.intensity = origObj.data.intensity;
		this->data.lightFocus = origObj.data.lightFocus;
	}

	return *this;
}

LightSource::~LightSource() {
	for (int i = 0; i < SHADOW_MAPS; i++) {
		if (shadowRenderTargetTexture[i]) {
			shadowRenderTargetTexture[i]->Release();
		}

		if (shadowRenderTargetView[i]) {
			shadowRenderTargetView[i]->Release();
		}

		if (shadowShaderResourceView[i]) {
			shadowShaderResourceView[i]->Release();
		}
	}
}

HRESULT LightSource::createShadowBuffer(ID3D11Device* device) {
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = S_WIDTH/4;
	textureDesc.Height = S_HEIGHT/4;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target textures.
	for (int i = 0; i < SHADOW_MAPS; i++) {
		if (FAILED(device->CreateTexture2D(&textureDesc, NULL, &shadowRenderTargetTexture[i]))) {
			return S_FALSE;
		}
	}

	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target views.
	for (int j = 0; j < SHADOW_MAPS; j++) {
		if (FAILED(device->CreateRenderTargetView(shadowRenderTargetTexture[j], &renderTargetViewDesc, &shadowRenderTargetView[j]))) {
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
	for (int k = 0; k < SHADOW_MAPS; k++) {
		if (FAILED(device->CreateShaderResourceView(shadowRenderTargetTexture[k], &shaderResourceViewDesc, &shadowShaderResourceView[k]))) {
			return S_FALSE;
		}
	}

	return S_OK;
}

HRESULT LightSource::Prepare(ID3D11DeviceContext* deviceContext, int index) const {
	if (index > SHADOW_MAPS-1)
		return S_FALSE;
	
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	deviceContext->ClearRenderTargetView(shadowRenderTargetView[index], clearColor);

	deviceContext->OMSetRenderTargets(1, &shadowRenderTargetView[index], nullptr);

	return S_OK;
}

HRESULT LightSource::Load(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pBuffer) const {
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	// Lock the light constant buffer so it can be written to
	HRESULT result = deviceContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	if (FAILED(result)) {
		return S_FALSE;
	}
	//Write data to constant buffer
	memcpy(mappedMemory.pData, (void*)&data, sizeof(LightData));

	// Unlock the constant buffer.
	deviceContext->Unmap(pBuffer, 0);

	return S_OK;
}

int LightSource::LightType() const {
	return data.type;
}

ID3D11ShaderResourceView* LightSource::ShadowMap(int index) const {
	if (index < SHADOW_MAPS)
		return shadowShaderResourceView[index];
	else
		return nullptr;
}

XMMATRIX LightSource::getView() const {
	return XMMatrixLookAtLH(
		data.position,
		data.direction,
		XMVectorSet(0,0,1,0)
	);
}

XMMATRIX LightSource::getProjection() const {
	return XMMatrixPerspectiveFovLH(
		(float)XM_PI*2,
		(float)S_WIDTH / (float)S_HEIGHT,
		0.1f,
		200.0f
	);
}

