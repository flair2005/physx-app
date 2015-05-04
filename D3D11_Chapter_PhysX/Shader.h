#pragma once

#include "DefHeaders.h"
#include "Render.h"

/* 
	��������������� �����, ���������� �������� �������� � ���������
	From Direct11 Tutorial @ zeswarchief
*/

class Shader {
private:
	HRESULT m_compileShaderFromFile(WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut);

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pLayout;
	ID3D11ShaderResourceView* m_pTexture;
	ID3D11SamplerState* m_pSampleState;

	D3D11_INPUT_ELEMENT_DESC* m_pLayoutFormat;
	unsigned int m_numLayout;
public:
	Shader();
	//������ ����� �� ������ �������� �� ������ createShader
	void addInputElementDesc(const char* semanticName, DXGI_FORMAT format);

	bool createShader(wchar_t* namevs, wchar_t* nameps, ID3D11Device* pDevice);
	bool loadTexture(const wchar_t* name, ID3D11Device* pDevice);

	void draw(ID3D11DeviceContext* pImmediateContext);
	void Close();
};