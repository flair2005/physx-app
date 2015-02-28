#include "Text.h"

Text::Text(Render* render, BitmapFont* font) {
	m_pRender = render;
	m_pFont = font;
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_numIndex = 0;
	m_numDrawIndex = 0;
	m_numVertex = 0;
	m_static = true;
	m_size = 0;
}

bool Text::init(const std::wstring& text, bool staticText, int charSize) {
	m_static = staticText;
	m_size = charSize;
	if(!m_initBuffers(text)) {
		return false;
	}

	return true;
}

bool Text::m_initBuffers(const std::wstring& text) {
	if(!m_size || (m_size < text.size())) {
		m_size = text.size();
	}

	m_numVertex = m_size * 4;
	m_numIndex = m_size * 6;
	m_numDrawIndex = text.size() * 6;
	if(m_numDrawIndex > m_numIndex) {
		m_numDrawIndex = m_numIndex;
	}

	VertexFont* vertex = new VertexFont[m_numVertex];
	if(!vertex) {
		return false;
	}

	unsigned long* indices = new unsigned long[m_numIndex];
	if(!indices) {
		return false;
	}

	m_pFont->buildVertexArray(vertex, m_numVertex, text.c_str());

	for(int i = 0; i < m_numIndex/6; i++) {
		indices[i*6 + 0] = i*4 + 0;
		indices[i*6 + 1] = i*4 + 1;
		indices[i*6 + 2] = i*4 + 2;
		indices[i*6 + 3] = i*4 + 0;
		indices[i*6 + 4] = i*4 + 3;
		indices[i*6 + 5] = i*4 + 1;
	}

	m_pVertexBuffer = Buffer::createVertexBuffer(m_pRender->getDevice(), sizeof(VertexFont)*m_numVertex, !m_static, vertex);
	if(!m_pVertexBuffer) {
		return false;
	}

	m_pIndexBuffer = Buffer::createIndexBuffer(m_pRender->getDevice(), sizeof(unsigned long)*m_numIndex, false, indices);
	if(!m_pIndexBuffer) {
		return false;
	}

	_DELETE_ARRAY(vertex);
	_DELETE_ARRAY(indices);

	return true;
}

bool Text::m_updateBuffer(const std::wstring& text) {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = m_pRender->getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result)) {
		return false;
	}

	VertexFont* verticesPtr = (VertexFont*)mappedResource.pData;
	
	m_pFont->buildVertexArray(verticesPtr, m_numVertex, text.c_str());

	m_pRender->getDeviceContext()->Unmap(m_pVertexBuffer, 0);

	return true;
}

void Text::draw(float r, float g, float b, float x, float y) {
	m_renderBuffers();
	m_pFont->draw(m_numDrawIndex, r, g, b, x, y);
}

void Text::m_renderBuffers() {
	unsigned int stride = sizeof(VertexFont);
	unsigned int offset = 0;
	m_pRender->getDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pRender->getDeviceContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pRender->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Text::Close() {
	_RELEASE(m_pVertexBuffer);
	_RELEASE(m_pIndexBuffer);
}

bool Text::setText(const std::wstring& text) {
	//Статический текст нельзя изменить
	if(m_static) {
		return false;
	}
	//Если текст меньше, чем число индексов, мы не должны рисовать ненужные
	m_numDrawIndex = text.size()*6;
	return m_updateBuffer(text);
}