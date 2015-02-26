#pragma once

#include "..\..\DefHeaders.h"
#include "..\..\Render\Render.h"
#include "..\..\Render\Shader.h"
#include "..\..\Util\Buffer.h"

/*
	Отвечает за создание и использование шрифтов
	Для создания объекта шрифта требуется файл в формате .fnt, который создается, например, программой BMFont
	К файлу *.fnt прилагается текстура *_0(1, 2, 3...).png
	From DirectX11 Tutorial @ zeswarchief
*/

struct VertexFont {
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};

class BitmapFont {
private:
	struct CharDesc {
		CharDesc() : srcX(0), srcY(0), srcW(0), srcH(0), xOff(0), yOff(0), xAdv(0) {}

		int srcX;
		int srcY;
		int srcW;
		int srcH;
		int xOff;
		int yOff;
		int xAdv;
	};

	struct ConstantBuffer {
		XMMATRIX WVP;
	};

	struct PixelBufferType {
		XMFLOAT4 pixelColor;
	};

	bool m_parse(char* fontFilename);
	void m_setShaderParameters(float r, float g, float b, float x, float y);

	Render* m_pRender;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pPixelBuffer;
	Shader* m_pShader;

	unsigned int m_widthTex;
	unsigned int m_heightTex;
	std::wstring m_file;
	std::map<int, CharDesc> m_chars;
public:
	BitmapFont(Render* render);

	bool init(char* fontFilename);
	void draw(unsigned int index, float r, float g, float b, float x, float y);
	void buildVertexArray(VertexFont* vert, int numvert, const wchar_t* sentence);
	void Close();
};