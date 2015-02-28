#include "Fps.h"

#pragma comment(lib, "winmm.lib")

void Fps::init() {
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
}

void Fps::frame() {
	m_count++;

	if(timeGetTime() >= (m_startTime + 1000)) {
		m_fps = m_count;
		fpsStr = intToStr(m_fps);
		m_count = 0;
		m_startTime = timeGetTime();	
	}
}

int Fps::getFps() {
	return m_fps;
}

//Вывод текста требует объект std::wstring
std::wstring Fps::getFpsStr() {
	return fpsStr;
}

std::wstring Fps::intToStr(int i) {
	swprintf(str, 255, L"%d", i);
	return str;
}