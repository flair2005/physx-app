#pragma once

#include "DefHeaders.h"
#include <MMSystem.h>
#include "Log.h"

class Fps {
private:
	int m_fps;
	int m_count;
	unsigned long m_startTime;
	std::wstring fpsStr;
	wchar_t str[255];		//Чтобы каждый раз не выделять память под временный массив для intToStr()
public:
	void init();
	void frame();
	int getFps();
	std::wstring getFpsStr();
	std::wstring intToStr(int i);
};