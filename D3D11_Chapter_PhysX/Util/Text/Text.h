#pragma once

#include "..\..\DefHeaders.h"
#include "BitmapFont.h"

class Text {
private:
	bool m_initBuffers(const std::wstring& text);
	void m_renderBuffers();
	bool m_updateBuffer(const std::wstring& text);

	Render* m_pRender;

	BitmapFont* m_pFont;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	int m_numIndex;
	int m_numDrawIndex;
	int m_numVertex;

	bool m_static;
	int m_size;
public:
	Text(Render* render, BitmapFont* font);

	//Text - текст, который нужно вынести
	//static - если true, то данный текст нельзя изменить
	//size - максимальное количество букв, которые можно вывести
	//		0 означает, что это число равно размеру text
	bool init(const std::wstring& text, bool staticText = true, int charSize = 0);
	void draw(float r, float g, float b, float x, float y);
	void Close();

	bool setText(const std::wstring& text);
};