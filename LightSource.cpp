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

	shadowRenderTargetTexture = nullptr;
	shadowDepthStencilView = nullptr;
	shadowShaderResourceView = nullptr;
}

LightSource::LightSource(const LightSource& origObj) {
	this->data.type = origObj.data.type;
	this->data.ambient = origObj.data.ambient;
	this->data.position = origObj.data.position;
	this->data.direction = origObj.data.direction;
	this->data.colour = origObj.data.colour;
	this->data.intensity = origObj.data.intensity;
	this->data.lightFocus = origObj.data.lightFocus;

	shadowRenderTargetTexture = nullptr;
	shadowDepthStencilView = nullptr;
	shadowShaderResourceView = nullptr;
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
	if (shadowRenderTargetTexture) {
		shadowRenderTargetTexture->Release();
	}
	if (shadowDepthStencilView) {
		shadowDepthStencilView->Release();
	}
	if (shadowShaderResourceView) {
		shadowShaderResourceView->Release();
	}
}

HRESULT LightSource::createShadowBuffer(ID3D11Device* device) {
	HRESULT result;

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;

	createViewport();

	if (data.type == L_POINT) {
		// Initialize the render target texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = S_WIDTH;
		textureDesc.Height = S_HEIGHT;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
	} else {
		// Initialize the render target texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = S_WIDTH;
		textureDesc.Height = S_HEIGHT;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
	}

	result = device->CreateTexture2D(&textureDesc, NULL, &shadowRenderTargetTexture);
	if (FAILED(result)) {
		return S_FALSE;
	}

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	depthStencilViewDesc.Texture2DArray.MipSlice = 0;
	depthStencilViewDesc.Texture2DArray.ArraySize = textureDesc.ArraySize;
	depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;

	result = device->CreateDepthStencilView(shadowRenderTargetTexture, &depthStencilViewDesc, &shadowDepthStencilView);
	if (FAILED(result)) {
		return S_FALSE;
	}

	ZeroMemory(&ShaderResourceViewDesc, sizeof(ShaderResourceViewDesc));

	ShaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	result = device->CreateShaderResourceView(shadowRenderTargetTexture, &ShaderResourceViewDesc, &shadowShaderResourceView);
	if (FAILED(result)) {
		return S_FALSE;
	}

	return S_OK;
}

void LightSource::createViewport() {
	vp.Width = (float)S_WIDTH;
	vp.Height = (float)S_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
}

void LightSource::prepareShadowRender(ID3D11DeviceContext* deviceContext) const {
	deviceContext->RSSetViewports(1, &this->vp);

	deviceContext->ClearDepthStencilView(shadowDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	deviceContext->OMSetRenderTargets(0, nullptr, shadowDepthStencilView);
}

HRESULT LightSource::loadShadowBuffers(ID3D11DeviceContext* deviceContext, ID3D11Buffer* buffer) const {
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	// Lock the light constant buffer so it can be written to
	HRESULT result = deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	if (FAILED(result)) {
		return S_FALSE;
	}
	//Write data to constant buffer
	memcpy(mappedMemory.pData, (void*)&data, sizeof(LightData));

	// Unlock the constant buffer.
	deviceContext->Unmap(buffer, 0);

	return S_OK;
}

HRESULT LightSource::loadLightBuffers(ID3D11DeviceContext* deviceContext, ID3D11Buffer* buffer) const {
	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	// Lock the light constant buffer so it can be written to
	HRESULT result = deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	if (FAILED(result)) {
		return S_FALSE;
	}
	//Write data to constant buffer
	memcpy(mappedMemory.pData, (void*)&data, sizeof(LightData));

	// Unlock the constant buffer.
	deviceContext->Unmap(buffer, 0);

	return S_OK;
}

int LightSource::getLightType() const {
	return data.type;
}

XMVECTOR LightSource::getOrigin() const {
	return data.position;
}

ID3D11ShaderResourceView* LightSource::getShadowMap() const {
	return shadowShaderResourceView;
}

XMMATRIX LightSource::getView(int index) const {
	XMMATRIX view;

	//fix position for L_DIRECTIONAL to place itself in front of camera always instead

	if (data.type == L_SPOT) {
		view = XMMatrixLookAtLH(
			data.position,
			data.direction,
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		);
	} else {
		XMFLOAT3* direction = new XMFLOAT3;
		XMStoreFloat3(direction, data.position);

		switch (index) {
		case 0:
			view = XMMatrixLookAtLH(
				data.position,
				XMVectorSet(direction->x+1, direction->y, direction->z, 0.0f),
				XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
			);
			break;
		case 1:

			view = XMMatrixLookAtLH(
				data.position,
				XMVectorSet(direction->x-1, direction->y, direction->z, 0.0f),
				XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
			);
			break;
		case 2:
			view = XMMatrixLookAtLH(
				data.position,
				XMVectorSet(direction->x, direction->y+1, direction->z, 0.0f),
				XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f)
			);
			break;
		case 3:
			view = XMMatrixLookAtLH(
				data.position,
				XMVectorSet(direction->x, direction->y-1, direction->z, 0.0f),
				XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)
			);
			break;
		case 4:
			view = XMMatrixLookAtLH(
				data.position,
				XMVectorSet(direction->x, direction->y, direction->z+1, 0.0f),
				XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
			);
			break;
		case 5:
			view = XMMatrixLookAtLH(
				data.position,
				XMVectorSet(direction->x, direction->y, direction->z-1, 0.0f),
				XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
			);
			break;
		}
	}
	view = XMMatrixTranspose(view);

	return view;
}

XMMATRIX LightSource::getProjection() const {
	XMMATRIX proj;

	switch (data.type) {
	case L_SPOT:
		proj = XMMatrixPerspectiveFovLH(
			(float)XM_PI * 0.5,
			(float)S_WIDTH*2 / (float)S_HEIGHT*2,
			0.1f,
			data.intensity * 1.1
		);
		break;
	case L_DIRECTIONAL:
		proj = XMMatrixOrthographicLH(
			(float)S_WIDTH,
			(float)S_HEIGHT,
			0.1f,
			data.intensity * 1.1
		);
		break;
	case L_POINT:
		proj = XMMatrixPerspectiveFovLH(
			(float)XM_PI * 0.5,
			(float)S_WIDTH*2 / (float)S_HEIGHT*2,
			0.1f,
			data.intensity * 1.1
		);
		break;
	}
	proj = XMMatrixTranspose(proj);

	return proj;
}