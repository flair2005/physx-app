#pragma once

#include "DefHeaders.h"
#include "Text.h"
#include "Render.h"
#include "BitmapFont.h"

/*
	Управляет всеми процессами, необходимыми для работы с текстом в приложении.
	Содержит неотсортированные ассоциативные массивы с объектами шрифтов, объектами статического текста и 
	объектами динамического текста.
	В качестве ключа для поиска по контейнерами используется объект std::string вида "key_*", чтобы отличать ключ от значения
	Отличие статического текста от динамического в том, что последний можно изменять во время работы программы, то есть
	метод setDynamicText() и Text::setText() не имеет эффекта для статического текста (да-да, тут нужно наследование).
	Сперва нужно создать шрифт (BitmapFont), указав имя файла (*.fnt). Если шрифт не находится в стандартной директории,
	нужно явно указать его расположение и название в виде "<метка_тома>:\\<директория>\\<директория...>\\<имя_файла>.fnt", 
	обязательно экранируя бэкслеши (просто два бэкслэша - \\), либо "<..\\<директория>\\<директория...>\\<имя_файла>.fnt",
	где "..\\" обозначает директорию проекта. В обоих случаях необходимо в файле шрифта изменить параметр "file="...".
	После создания объекта шрифта и занесения его в контейнер fontMap, можно создать объект текста, указав ключ нужного шрифта,
	ключ создаваемого текста и сам текст (в случае с динамическим текстом, это будет начальное значение - оно все равно потом поменяется)
	Операции с объектами шрифтов и текстов можно удалять и изменять (динамический текст) - в этом случае, вызывая соответствующий
	метод необходимо указать корректный ключ, который был определен во время создания этих объектов.
	Когда все приготовления окончены, нужно вызвать метод draw() во фреймворке перед вызовом Render::draw()
*/

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

	//При добавлении нового шрифта или текста мы заполняем соответствующую структуру и добавляем ее в контейнер
	void addStaticText(std::string textKey, std::string fontKey, std::wstring str,
												float r, float g, float b, float x, float y);
	void addDynamicText(std::string textKey, std::string fontKey, std::wstring str,
												float r, float g, float b, float x, float y, int charSize = 0);
	bool addFont(const std::string key, char* fileName);

	bool deleteStaticText(const std::string key);
	bool deleteDynamicText(const std::string key);
	bool deleteFont(const std::string key);

	void setDynamicText(const std::string key, std::wstring text);		//Не имеет смысла для статического текста

	void drawText();

	void Close();
};