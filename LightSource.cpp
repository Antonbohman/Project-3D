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

	setViewport();
	setViews();
	setProjection();
	setViewProjection();
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

	setViewport();
	setViews();
	setProjection();
	setViewProjection();
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

		setViewport();
		setViews();
		setProjection();
		setViewProjection();
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

void LightSource::setViewport() {
	switch (data.type) {
	case L_SPOT:
		vp.Width = (float)S_WIDTH;
		vp.Height = (float)S_HEIGHT;
		break;
	case L_POINT:
		vp.Width = (float)S_C_WIDTH;
		vp.Height = (float)S_C_HEIGHT;
		break;
	case L_DIRECTIONAL:
		vp.Width = (float)S_DIR_WIDTH;
		vp.Height = (float)S_DIR_HEIGHT;
		break;
	}

	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
}

void LightSource::setViews() {
	XMVECTOR lookUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR lookDown = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

	switch (data.type) {
	case L_SPOT:
		views[0] = XMMatrixLookAtLH(
			data.position,
			data.direction,
			lookUp
		);
		views[1] = views[0];
		views[2] = views[0];
		views[3] = views[0];
		views[4] = views[0];
		views[5] = views[0];
		break;
	case L_POINT:
		views[0] = XMMatrixLookAtLH(
			data.position,
			data.direction,
			lookUp
		);
		views[1] = views[0];
		views[2] = views[0];
		views[3] = views[0];
		views[4] = views[0];
		views[5] = views[0];
		break;
	case L_DIRECTIONAL:
		XMFLOAT3 * direction = new XMFLOAT3;
		XMStoreFloat3(direction, data.position);

		views[0] = XMMatrixLookAtLH(
			data.position,
			XMVectorSet(direction->x + 100, direction->y, direction->z, 0.0f),
			lookUp
		);
		views[1] = XMMatrixLookAtLH(
			data.position,
			XMVectorSet(direction->x - 100, direction->y, direction->z, 0.0f),
			lookUp
		);
		views[2] = XMMatrixLookAtLH(
			data.position,
			XMVectorSet(direction->x, direction->y, direction->z + 100, 0.0f),
			lookUp
		);
		views[3] = XMMatrixLookAtLH(
			data.position,
			XMVectorSet(direction->x, direction->y, direction->z - 100, 0.0f),
			lookUp
		);
		views[4] = XMMatrixLookAtLH(
			data.position,
			XMVectorSet(direction->x, direction->y + 100, direction->z, 0.0f),
			lookUp
		);
		views[5] = XMMatrixLookAtLH(
			data.position,
			XMVectorSet(direction->x, direction->y - 100, direction->z, 0.0f),
			lookUp
		);
		break;
	}

	for (int i = 0; i < 6; i++) {
		views[i] = XMMatrixTranspose(views[i]);
	}
}

void LightSource::setProjection() {
	switch (data.type) {
	case L_SPOT:
		projection = XMMatrixPerspectiveFovLH(
			(float)XM_PI * 0.5,
			(float)S_WIDTH * 2 / (float)S_HEIGHT * 2,
			0.1f,
			max(data.intensity * 1.1f, 1.0f)
		);
		break;
	case L_POINT:
		projection = XMMatrixPerspectiveFovLH(
			(float)XM_PI * 0.5,
			(float)S_C_WIDTH * 2 / (float)S_C_HEIGHT * 2,
			0.1f,
			max(data.intensity * 1.1f, 1.0f)
		);
		break;
	case L_DIRECTIONAL:
		projection = XMMatrixOrthographicLH(
			(float)S_DIR_WIDTH,
			(float)S_DIR_HEIGHT,
			0.1f,
			max(data.intensity * 1.1f, 1.0f)
		);
		break;
	}
	projection = XMMatrixTranspose(projection);
}

void LightSource::setViewProjection() {
	XMMATRIX p = this->projection;
	XMMATRIX v = this->views[0];

	data.viewProjection = XMMatrixMultiply(p, v);
	v = this->views[1];
	data.rotatedViewProjection[0] = XMMatrixMultiply(p, v);
	v = this->views[2];
	data.rotatedViewProjection[1] = XMMatrixMultiply(p, v);
	v = this->views[3];
	data.rotatedViewProjection[2] = XMMatrixMultiply(p, v);
	v = this->views[4];
	data.rotatedViewProjection[3] = XMMatrixMultiply(p, v);
	v = this->views[5];
	data.rotatedViewProjection[4] = XMMatrixMultiply(p, v);
}

HRESULT LightSource::createShadowBuffer(ID3D11Device* device) {
	HRESULT result;

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;

	switch (data.type) {
	case L_SPOT:
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
		break;
	case L_POINT:
		// Initialize the render target texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = S_C_WIDTH;
		textureDesc.Height = S_C_HEIGHT;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		break;
	case L_DIRECTIONAL:
		// Initialize the render target texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = S_DIR_WIDTH;
		textureDesc.Height = S_DIR_HEIGHT;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		break;
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

XMMATRIX LightSource::getViewProjection(int index) const {
	if (index > 5)
		index = 0;

	if (index == 0)
		return data.viewProjection;
	else
		return data.rotatedViewProjection[index];
}