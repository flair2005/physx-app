#pragma once

#include "..\..\DefHeaders.h"
#include "Text.h"
#include "..\..\Render\Render.h"
#include "BitmapFont.h"

class TextManager {
private:
	struct StaticTextContainer {
		Text* text;
		float r;
		float g;
		float b;
		float x;
		float y;
		bool isStatic;
	};

	struct DynamicTextContainer {
		Text* text;
		float r;
		float g;
		float b;
		float x;
		float y;
		bool isStatic;
	};

	std::unordered_map<std::string, StaticTextContainer> staticTextMap;
	std::unordered_map<std::string, DynamicTextContainer> dynamicTextMap;

	std::unordered_map<std::string, BitmapFont*> fontMap;

	Render* m_pRender;
public:
	TextManager(Render* pRender);
	~TextManager();

	void addStaticText(std::string textKey, std::string fontKey, std::wstring str,
												float r, float g, float b, float x, float y);
	void addDynamicText(std::string textKey, std::string fontKey, std::wstring str,
												float r, float g, float b, float x, float y, int charSize = 0);
	bool addFont(const std::string key, char* fileName);

	bool deleteStaticText(const std::string key);
	bool deleteDynamicText(const std::string key);
	bool deleteFont(const std::string key);

	void setDynamicText(const std::string key, std::wstring text);

	void drawText();

	void Close();
};