#include "Shader.h"

#define MAXLAYOUT 8

Shader::Shader(Render* render) {
	m_pRender = render;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pLayout = nullptr;
	m_pSampleState = nullptr;
	m_pTexture = nullptr;
	m_pLayoutFormat = nullptr;
	m_numLayout = 0;
}

void Shader::addInputElementDesc(const char* semanticName, DXGI_FORMAT format) {
	if(!m_numLayout) {
		m_pLayoutFormat = new D3D11_INPUT_ELEMENT_DESC[MAXLAYOUT];
		if(!m_pLayoutFormat) {
			return;
		}
	} else if(m_numLayout >= MAXLAYOUT) {
		return;
	}

	D3D11_INPUT_ELEMENT_DESC& layout = m_pLayoutFormat[m_numLayout];
	layout.SemanticName = semanticName;
	layout.SemanticIndex = 0;
	layout.Format = format;
	layout.InputSlot = 0;
	if(!m_numLayout) {
		layout.AlignedByteOffset = 0;
	} else {
		layout.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	}
	layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout.InstanceDataStepRate = 0;

	m_numLayout++;
}

bool Shader::createShader(wchar_t* namevs, wchar_t* nameps) {
	HRESULT hr = S_OK;
	ID3DBlob* vertexShaderBuffer = nullptr;
	hr = m_compileShaderFromFile(namevs, "VS", "vs_4_0", &vertexShaderBuffer);
	if(FAILED(hr)) {
		Log::get()->err("Не удалось загрузить вершинный шейдер %ls", namevs);
		return false;
	}

	ID3DBlob* pixelShaderBuffer = nullptr;
	hr = m_compileShaderFromFile(nameps, "PS", "ps_4_0", &pixelShaderBuffer);
	if(FAILED(hr)) {
		Log::get()->err("Не удалось загрузить пиксельный шейдер %ls", nameps);
		return false;
	}

	hr = m_pRender->getDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
													vertexShaderBuffer->GetBufferSize(), NULL,
													&m_pVertexShader);
	if(FAILED(hr)) {
		Log::get()->err("Не удалось создать вершинный шейдер");
		return false;
	}

	hr = m_pRender->getDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
													pixelShaderBuffer->GetBufferSize(), NULL,
													&m_pPixelShader);
	if(FAILED(hr)) {
		Log::get()->err("Не удалось создать пиксельный шейдер");
		return false;
	}

	hr = m_pRender->getDevice()->CreateInputLayout(m_pLayoutFormat, m_numLayout, vertexShaderBuffer->GetBufferPointer(),
													vertexShaderBuffer->GetBufferSize(), &m_pLayout);
	if(FAILED(hr)) {
		Log::get()->err("Не удалось создать формат ввода");
		return false;
	}

	_DELETE_ARRAY(m_pLayoutFormat);

	_RELEASE(vertexShaderBuffer);
	_RELEASE(pixelShaderBuffer);

	return true;
}

HRESULT Shader::m_compileShaderFromFile(WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut) {
	HRESULT hr = S_OK;

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DX11CompileFromFile(fileName, NULL, NULL, entryPoint, shaderModel, shaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if(FAILED(hr) && pErrorBlob) {
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
	}
	_RELEASE(pErrorBlob);

	return hr;
}

bool Shader::loadTexture(const wchar_t* name) {
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(m_pRender->getDevice(), name, NULL, NULL, &m_pTexture, NULL);
	if(FAILED(hr)) {
		Log::get()->err("Не удалось загрузить текстуру %ls", name);
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_pRender->getDevice()->CreateSamplerState(&samplerDesc, &m_pSampleState);
	if(FAILED(hr)) {
		Log::get()->err("Не удалось создать sample state");
		return false;
	}

	return true;
}

void Shader::draw() {
	m_pRender->getDeviceContext()->IASetInputLayout(m_pLayout);
	m_pRender->getDeviceContext()->VSSetShader(m_pVertexShader, NULL, 0);
	m_pRender->getDeviceContext()->PSSetShader(m_pPixelShader, NULL, 0);
	if(m_pTexture) {
		m_pRender->getDeviceContext()->PSSetShaderResources(0, 1, &m_pTexture);
	}
	if(m_pSampleState) {
		m_pRender->getDeviceContext()->PSSetSamplers(0, 1, &m_pSampleState);
	}
}

void Shader::Close() {
	_RELEASE(m_pVertexShader);
	_RELEASE(m_pPixelShader);
	_RELEASE(m_pLayout);
	_RELEASE(m_pSampleState);
	_RELEASE(m_pTexture);
}