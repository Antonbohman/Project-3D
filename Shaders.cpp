#include "Shaders.h"

HRESULT CreateShaders() {
	//create deferred shaders 
	if (FAILED(CreateDeferredVS())) return S_FALSE;
	if (FAILED(CreateDeferredGS())) return S_FALSE;
	if (FAILED(CreateDeferredPS())) return S_FALSE;
	//if (FAILED(CreateDeferredCS())) return S_FALSE;

	//create light shaders
	if (FAILED(CreateLightVS())) return S_FALSE;
	if (FAILED(CreateLightPS())) return S_FALSE;

	return S_OK;
}

void DestroyShaders() {
	gVertexShader->Release();
	gGeometryShader->Release();
	gPixelShader->Release();
	//gComputeShader->Release();

	gLightVertexShader->Release();
	gLightPixelShader->Release();

	gVertexLayout->Release();
	gLightVertexLayout->Release();
}

HRESULT CreateDeferredVS() {
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pVS = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"shaders/Vertex.hlsl",   // filename
		nullptr,		  // optional macros
		nullptr,		  // optional include files
		"VS_main",		  // entry point
		"vs_5_0",		  // shader model (target)
		D3DCOMPILE_DEBUG, // shader compile options (DEBUGGING)
		0,				  // IGNORE...DEPRECATED.
		&pVS,			  // double pointer to ID3DBlob		
		&errorBlob		  // pointer for Error Blob messages.
	);

	if (FAILED(result)) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
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

	if (errorBlob) errorBlob->Release();
	pVS->Release();

	return S_OK;
}

HRESULT CreateDeferredGS() {
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pGS = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"shaders/Geometry.hlsl",	// filename
		nullptr,			// optional macros
		nullptr,			// optional include files
		"GS_main",			// entry point
		"gs_5_0",			// shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,					// effect compile options
		&pGS,				// double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	if (FAILED(result)) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
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

	if (errorBlob) errorBlob->Release();
	pGS->Release();

	return S_OK;
}

HRESULT CreateDeferredPS() {
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pPS = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"shaders/Fragment.hlsl",   // filename
		nullptr,		    // optional macros
		nullptr,		    // optional include files
		"PS_main",		    // entry point
		"ps_5_0",		    // shader model (target)
		D3DCOMPILE_DEBUG,	// shader compile options
		0,				    // effect compile options
		&pPS,			    // double pointer to ID3DBlob		
		&errorBlob			// pointer for Error Blob messages.
	);

	if (FAILED(result)) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
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

	if (errorBlob) errorBlob->Release();
	pPS->Release();

	return S_OK;
}

HRESULT CreateDeferredCS() {
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pCS = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"Compute.hlsl",   // filename
		nullptr,		  // optional macros
		nullptr,		  // optional include files
		"CS_main",		  // entry point
		"cs_5_0",		  // shader model (target)
		D3DCOMPILE_DEBUG, // shader compile options (DEBUGGING)
		0,				  // IGNORE...DEPRECATED.
		&pCS,			  // double pointer to ID3DBlob
		&errorBlob		  // pointer for Error Blob messages.
	);

	if (FAILED(result)) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pCS)
			pCS->Release();
		return result;
	}

	/*gDevice->CreateComputeShader(
		pCS->GetBufferPointer(),
		pCS->GetBufferSize(),
		nullptr,
		&gPixelShader
	);*/

	if (errorBlob) errorBlob->Release();
	pCS->Release();

	return S_OK;
}

HRESULT CreateLightVS() {
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pVS = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"shaders/LightVertex.hlsl",   // filename
		nullptr,		  // optional macros
		nullptr,		  // optional include files
		"VS_light",		  // entry point
		"vs_5_0",		  // shader model (target)
		D3DCOMPILE_DEBUG, // shader compile options (DEBUGGING)
		0,				  // IGNORE...DEPRECATED.
		&pVS,			  // double pointer to ID3DBlob		
		&errorBlob		  // pointer for Error Blob messages.
	);

	if (FAILED(result)) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
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
		&gLightVertexShader
	);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",					 // "semantic" name in shader
			0,							 // "semantic" index (not used)
			DXGI_FORMAT_R32G32_FLOAT,    // size of ONE element (2 floats)
			0,							 // input slot
			0,							 // offset of first element
			D3D11_INPUT_PER_VERTEX_DATA, // specify data PER vertex
			0							 // used for INSTANCING (ignore)
		},
	};

	gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gLightVertexLayout);

	if (errorBlob) errorBlob->Release();
	pVS->Release();

	return S_OK;
}

HRESULT CreateLightPS() {
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pPS = nullptr;

	HRESULT result = D3DCompileFromFile(
		L"shaders/LightFragment.hlsl",   // filename
		nullptr,		  // optional macros
		nullptr,		  // optional include files
		"PS_light",		  // entry point
		"ps_5_0",		  // shader model (target)
		D3DCOMPILE_DEBUG, // shader compile options (DEBUGGING)
		0,				  // IGNORE...DEPRECATED.
		&pPS,			  // double pointer to ID3DBlob		
		&errorBlob		  // pointer for Error Blob messages.
	);

	if (FAILED(result)) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
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
		&gLightPixelShader
	);

	if (errorBlob) errorBlob->Release();
	pPS->Release();

	return S_OK;
}

void SetDeferredShaders() {
	gDeviceContext->OMSetRenderTargets(G_BUFFER, gRenderTargetViewArray, gDepth);

	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShader, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);
	//gDeviceContext->CSSetShader(gComputeShader, nullptr, 0);

	UINT32 vertexSize = sizeof(TriangleVertex);
	UINT32 offset = 0;

	// specify which vertex buffer to use next.
	//gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBufferMap, &vertexSize, &offset);

	// specify the topology to use when drawing
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// specify the IA Layout (how is data passed)
	gDeviceContext->IASetInputLayout(gVertexLayout);
}

void SetLightShaders() {
	gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, gDepth);

	gDeviceContext->VSSetShader(gLightVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gLightPixelShader, nullptr, 0);
	//gDeviceContext->CSSetShader(nullptr, nullptr, 0);

	UINT32 vertexSize = sizeof(PositionVertex);
	UINT32 offset = 0;

	// specify which vertex buffer to use next.
	gDeviceContext->IASetVertexBuffers(0, 1, &gDeferredQuadBuffer, &vertexSize, &offset);

	// specify the topology to use when drawing
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// specify the IA Layout (how is data passed)
	gDeviceContext->IASetInputLayout(gLightVertexLayout);
}