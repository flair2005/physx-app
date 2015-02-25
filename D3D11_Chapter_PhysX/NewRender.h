/*#pragma once

#include "DefHeaders.h"
#include "Physics.h"
#include "Render.h"

class NewRender : protected Render {
private:
	HRESULT m_compileShaderFromFile(WCHAR* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut);

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	Physics* m_pPhysics;
public:
	NewRender();
	
	bool init(HWND hwnd);
	bool draw();
	void Close();
};*/