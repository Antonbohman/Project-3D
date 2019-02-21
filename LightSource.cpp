#include "LightSource.h"

LightSource::LightSource(const int type, const XMVECTOR position, 
						const XMVECTOR direction, const XMVECTOR colour, 
						const float intensity, const float angleX, const float angleY) {
	data.byte2 = 0;

	data.type = type;
	data.position = position;
	data.direction = direction;
	data.colour = colour;
	data.intensity = intensity;
	data.angleX = angleX;
	data.angleY = angleY;
}

LightSource::LightSource(const LightSource& origObj) {
	this->data.byte2 = 0;

	this->data.type = origObj.data.type;
	this->data.position = origObj.data.position;
	this->data.direction = origObj.data.direction;
	this->data.colour = origObj.data.colour;
	this->data.intensity = origObj.data.intensity;
	this->data.angleX = origObj.data.angleX;
	this->data.angleY = origObj.data.angleY;
}

LightSource& LightSource::operator=(const LightSource& origObj) {
	if (this != &origObj) {
		this->data.byte2 = 0;

		this->data.type = origObj.data.type;
		this->data.position = origObj.data.position;
		this->data.direction = origObj.data.direction;
		this->data.colour = origObj.data.colour;
		this->data.intensity = origObj.data.intensity;
		this->data.angleX = origObj.data.angleX;
		this->data.angleY = origObj.data.angleY;
	}

	return *this;
}

LightSource::~LightSource() {
}


bool LightSource::Load(ID3D11DeviceContext* deviceContext, ID3D11Buffer* pBuffer) const {

	D3D11_MAPPED_SUBRESOURCE mappedMemory;

	// Lock the light constant buffer so it can be written to
	HRESULT result = deviceContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedMemory);
	if (FAILED(result)) {
		return false;
	}
	//Write data to constant buffer
	memcpy(mappedMemory.pData, (void*)&data, sizeof(LightData));

	// Unlock the constant buffer.
	deviceContext->Unmap(pBuffer, 0);

	return true;
}