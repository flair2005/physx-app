#pragma once

#include "InputCodes.h"
#include "Log.h"
#include "InputListener.h"


	class InputMgr {
	private:
		//������� �������� ����
		void m_eventCursor();
		//������� ������ ����
		void m_eventMouse(const eMouseKeyCodes keyCode, bool press);
		//������� �������� ��������
		void m_mouseWheel(int value);
		//��������� ������� �������
		void m_eventKey(const eKeyCodes keyCode, const wchar_t ch, bool press);

		std::list<InputListener*> m_listener;

		RECT m_windowRect;
		int m_curX;
		int m_curY;
		int m_mouseWheelValue;
	public:
		void init();
		void Close();

		void run(const UINT& msg, WPARAM wParam, LPARAM lParam);

		void addListener(InputListener* listener);

		//���� ����
		void setWinRect(const RECT& winRect);
	};

