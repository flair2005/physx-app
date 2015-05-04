#include "BitmapFont.h"

inline wchar_t* charToWChar(char *mbString)
{ 
	int len = 0; 
	len = (int)strlen(mbString) + 1; 
	wchar_t *ucString = new wchar_t[len]; 
	mbstowcs(ucString, mbString, len); 
	return ucString; 
}

BitmapFont::BitmapFont(Render* render) {
	m_pRender = render;
	m_pConstantBuffer = nullptr;
	m_pPixelBuffer = nullptr;
	m_pShader = nullptr;
	m_widthTex = 0;
	m_heightTex = 0;
}

bool BitmapFont::init(char* fontFileName) {
	if(!m_parse(fontFileName)) {
		return false;
	}
	m_pShader = new Shader();
	if(!m_pShader) {
		return false;
	}
	if(!m_pShader->loadTexture(m_file.c_str(), m_pRender->getDevice())) {
		return false;
	}
	m_pShader->addInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	m_pShader->addInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	if(!m_pShader->createShader(L"Shaders\\BitmapFont.vs", L"Shaders\\BitmapFont.ps", m_pRender->getDevice())) {
		return false;
	}

	m_pConstantBuffer = Buffer::createConstantBuffer(m_pRender->getDevice(), sizeof(ConstantBuffer), false);
	if(!m_pConstantBuffer) {
		return false;
	}

	m_pPixelBuffer = Buffer::createConstantBuffer(m_pRender->getDevice(), sizeof(PixelBufferType), false);
	if(!m_pPixelBuffer) {
		return false;
	}

	return true;
}

bool BitmapFont::m_parse(char* fontFilename) {
	std::ifstream fin;
	fin.open(fontFilename);
	if(fin.fail()) {
		return false;
	}

	std::string line;
	std::string read, key, value;
	size_t i;
	while(!fin.eof()) {
		std::stringstream lineStream;
		std::getline(fin, line);
		lineStream << line;

		lineStream >> read;
		if(read == "common") {
			while(!lineStream.eof()) {
				std::stringstream converter;
				lineStream >> read;
				i = read.find('=');
				key = read.substr(0, i);
				value = read.substr(i + 1);

				converter << value;
				if(key == "scaleW") {
					converter >> m_widthTex;
				} else if (key == "scaleH") {
					converter >> m_heightTex;
				}
			}
		} else if(read == "page") {
			while(!lineStream.eof()) {
				std::stringstream converter;
				lineStream >> read;
				i = read.find('=');
				key = read.substr(0, i);
				value = read.substr(i + 1);

				std::string str;
				converter << value;
				if(key == "file") {
					converter >> str;
					wchar_t* name = charToWChar((char*)str.substr(1, value.length()-2).c_str());
					m_file = name;
					_DELETE_ARRAY(name);
				}
			}
		} else if(read == "char") {
			unsigned short charID = 0;
			CharDesc chard;

			while(!lineStream.eof()) {
				std::stringstream converter;
				lineStream >> read;
				i = read.find('=');
				key = read.substr(0, i);
				value = read.substr(i + 1);

				converter << value;
				if(key == "id")				converter >> charID;
				else if(key == "x")			converter >> chard.srcX;
				else if(key == "y")			converter >> chard.srcY;
				else if(key == "width")		converter >> chard.srcW;
				else if(key == "height")	converter >> chard.srcH;
				else if(key == "xoffset")	converter >> chard.xOff;
				else if(key == "yoffset")	converter >> chard.yOff;
				else if(key == "xadvance")	converter >> chard.xAdv;
			}
			m_chars.insert(std::pair<int, CharDesc>(charID, chard));
		}
	}
	fin.close();

	return true;
}

void BitmapFont::buildVertexArray(VertexFont* vertex, int numVert, const wchar_t* sentence) {
	int numLetters = (int)wcslen(sentence);
	//Следим чтобы число букв не было больше числа вершин
	if(numLetters*4 > numVert) {
		numLetters = numVert/4;
	}

	float drawX = (float) m_pRender->getWidth()/2 * (-1);
	float drawY = (float) m_pRender->getHeight()/2;

	int index = 0;
	for(int i = 0; i < numLetters; i++) {
		float charX = m_chars[sentence[i]].srcX;
		float charY = m_chars[sentence[i]].srcY;
		float width = m_chars[sentence[i]].srcW;
		float height = m_chars[sentence[i]].srcH;
		float offsetX = m_chars[sentence[i]].xOff;
		float offsetY = m_chars[sentence[i]].yOff;
		
		float left = drawX + offsetX;
		float right = left + width;
		float top = drawY - offsetY;
		float bottom = top - height;
		float lefttex = charX/m_widthTex;
		float righttex = (charX + width)/m_widthTex;
		float toptex = charY/m_heightTex;
		float bottomtex = (charY + height)/m_heightTex;

		vertex[index].pos = XMFLOAT3(left, top, 0.0f);
		vertex[index].tex = XMFLOAT2(lefttex, toptex);
		index++;
		vertex[index].pos = XMFLOAT3(right, bottom, 0.0f);
		vertex[index].tex = XMFLOAT2(righttex, bottomtex);
		index++;
		vertex[index].pos = XMFLOAT3(left, bottom, 0.0f);
		vertex[index].tex = XMFLOAT2(lefttex, bottomtex);
		index++;
		vertex[index].pos = XMFLOAT3(right, top, 0.0f);
		vertex[index].tex = XMFLOAT2(righttex, toptex);
		index++;

		drawX += m_chars[sentence[i]].xAdv;
	}
}

void BitmapFont::draw(unsigned int index, float r, float g, float b, float x, float y) {
	m_setShaderParameters(r, g, b, x, y);

	m_pShader->draw(m_pRender->getDeviceContext());
	m_pRender->getDeviceContext()->DrawIndexed(index, 0, 0);
}

void BitmapFont::m_setShaderParameters(float r, float g, float b, float x, float y) {
	XMMATRIX objmatrix = XMMatrixTranslation(x, -y, 0);
	XMMATRIX wvp = objmatrix * m_pRender->getOrtho();
	ConstantBuffer cb;
	cb.WVP = XMMatrixTranspose(wvp);
	m_pRender->getDeviceContext()->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb, 0, 0);

	m_pRender->getDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	PixelBufferType pb;
	pb.pixelColor = XMFLOAT4(r, g, b, 1.0f);
	m_pRender->getDeviceContext()->UpdateSubresource(m_pPixelBuffer, 0, NULL, &pb, 0, 0);

	m_pRender->getDeviceContext()->PSSetConstantBuffers(0, 1, &m_pPixelBuffer);
}

void BitmapFont::Close() {
	_RELEASE(m_pConstantBuffer);
	_RELEASE(m_pPixelBuffer);
	_CLOSE(m_pShader);
}

