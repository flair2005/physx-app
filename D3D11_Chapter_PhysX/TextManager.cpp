#include "TextManager.h"

TextManager::TextManager(Render* pRender) {
	if(!pRender) {
		Log::get()->err("pRender == nullptr int TextManager(Render* pRender)");
		return;
	}
	m_pRender = pRender;
	staticTextMap.clear();
	dynamicTextMap.clear();
	fontMap.clear();
}

TextManager::~TextManager() {

}

bool TextManager::addFont(std::string key, char* fileName) {
	if(key == "\0" && key == "\n") {
		Log::get()->err("Font Create Fail: invalid key");
		return false;
	}

	BitmapFont* font = new BitmapFont(m_pRender);
	if(!font) {
		Log::get()->err("BitmapFont Allocation Failed");
		return false;
	}

	if(!font->init(fileName)) {
		Log::get()->err("font->init(%s) Failed", fileName);
		return false;
	}

	fontMap[key] = font;

	return true;
}

void TextManager::addStaticText(std::string textKey, std::string fontKey, std::wstring str,
												float r, float g, float b, float x, float y) {
	if(fontMap.count(fontKey) == 0) {
		Log::get()->err("Font with Key \"%s\" is not found", fontKey);
		return;
	}
	Text* newText = new Text(m_pRender, fontMap.at(fontKey));

	newText->init(str);

	StaticTextContainer stc;
	stc.isStatic = true;
	stc.text = newText;
	stc.r = r;
	stc.g = g;
	stc.b = b;
	stc.x = x;
	stc.y = y;

	staticTextMap[textKey] = stc;
}

void TextManager::addDynamicText(std::string textKey, std::string fontKey, std::wstring str,
												float r, float g, float b, float x, float y, int charSize) {
	if(fontMap.count(fontKey) == 0) {
		Log::get()->err("Font with Key \"%s\" is not found", fontKey);
		return;
	}
	Text* newText = new Text(m_pRender, fontMap.at(fontKey));

	newText->init(str, false, charSize);

	DynamicTextContainer dtc;
	dtc.text = newText;
	dtc.isStatic = false;
	dtc.r = r;
	dtc.g = g;
	dtc.b = b;
	dtc.x = x;
	dtc.y = y;

	dynamicTextMap[textKey] = dtc;
}

void TextManager::drawText() {
	if(!m_pRender->hasInited()) {
		Log::get()->err("TextManager::drawText() - Render не был инициализирован");
		return;
	}
						
	if(staticTextMap.size() == 0) {
		return;
	}
	m_pRender->turnOnAlphaBlending();						//Включаем прозрачность
	std::unordered_map<std::string, StaticTextContainer>::pointer ps;
	for(std::unordered_map<std::string, StaticTextContainer>::iterator it = staticTextMap.begin();
				it != staticTextMap.end(); it++) {
		ps = &*it;
		ps->second.text->draw(ps->second.r, ps->second.g, ps->second.b, ps->second.x, ps->second.y);
	}
	m_pRender->turnOffAlphaBlending();						//Выключаем прозрачность

	if(dynamicTextMap.size() == 0) {
		return;
	}
	m_pRender->turnOnAlphaBlending();						//Включаем прозрачность
	std::unordered_map<std::string, DynamicTextContainer>::pointer pd;
	for(std::unordered_map<std::string, DynamicTextContainer>::iterator it = dynamicTextMap.begin();
				it != dynamicTextMap.end(); it++) {
		pd = &*it;
		pd->second.text->draw(pd->second.r, pd->second.g, pd->second.b, pd->second.x, pd->second.y);
	}
	m_pRender->turnOffAlphaBlending();						//Выключаем прозрачность


	//m_pText1->setText(m_pFps->getFpsStr());				//Выводим количество фпс
//	m_pText1->draw(1.0f, 1.0f, 0.0f, 80.0f, 10.0f);

	//Text* pr = staticTextMap

	//m_pText2->draw(1.0f, 1.0f, 0.0f, 10.0f, 10.f);		//Статическая надпись "FPS:"

	//m_pText3->draw(0.0f, 1.0f, 0.0f, 10.0f, 40.0f);

	//m_pText4->setText(m_pScene->getActorPos());
	//m_pText4->draw(0.0f, 1.0f, 0.0f, 160.0f, 40.0f);
}

void TextManager::setDynamicText(std::string key, std::wstring text) {
	if(dynamicTextMap.count(key) == 0) {
		Log::get()->err("DynamicText with Key \"%s\" is not found in setDynamicText(...)", key);
		return;
	}

	dynamicTextMap.at(key).text->setText(text);
}

void TextManager::Close() {
	fontMap.clear();
	staticTextMap.clear();
	dynamicTextMap.clear();
}