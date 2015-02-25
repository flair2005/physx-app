#include "InputMgr.h"

	void InputMgr::init() {
		m_mouseWheelValue = m_curX = m_curY = 0;
		Log::get()->debug("InputMgr init");
	}

	void InputMgr::Close() {
		if(!m_listener.empty()) {
			m_listener.clear();
		}
		Log::get()->debug("InputMgr close");
	}

	void InputMgr::setWinRect(const RECT& winrect) {
		m_windowRect.left = winrect.left;
		m_windowRect.right = winrect.right;
		m_windowRect.top = winrect.top;
		m_windowRect.bottom = winrect.bottom;
	}

	void InputMgr::addListener(InputListener* listener) {
		m_listener.push_back(listener);
	}

	void InputMgr::run(const UINT& msg, WPARAM wParam, LPARAM lParam) {
		if(m_listener.empty()) {
			return;
		}

		eKeyCodes keyIndex;
		wchar_t buffer[1];
		BYTE lpKeyState[256];

		m_eventCursor();				//Событие движения мыши
		switch(msg) {
		case WM_KEYDOWN:
			keyIndex = static_cast<eKeyCodes>(wParam);
			GetKeyboardState(lpKeyState);
			ToUnicode(wParam, HIWORD(lParam)&0xFF, lpKeyState, buffer, 1, 0);
			m_eventKey(keyIndex, buffer[0], true);
			break;
		case WM_KEYUP:
			keyIndex = static_cast<eKeyCodes>(wParam);
			GetKeyboardState(lpKeyState);
			ToUnicode(wParam, HIWORD(lParam)&0xFF, lpKeyState, buffer, 1, 0);
			m_eventKey(keyIndex, buffer[0], false);
			break;
		case WM_LBUTTONDOWN:
			m_eventMouse(MOUSE_LEFT, true);
			break;
		case WM_LBUTTONUP:
			m_eventMouse(MOUSE_LEFT, false);
			break;
		case WM_RBUTTONDOWN:
			m_eventMouse(MOUSE_RIGHT, true);
			break;
		case WM_RBUTTONUP:
			m_eventMouse(MOUSE_RIGHT, false);
			break;
		case WM_MBUTTONDOWN:
			m_eventMouse(MOUSE_MIDDLE, true);
			break;
		case WM_MBUTTONUP:
			m_eventMouse(MOUSE_MIDDLE, false);
			break;
		case WM_MOUSEWHEEL:
			m_mouseWheel((int)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			break;
		}
	}

	void InputMgr::m_eventCursor() {
		POINT position;
		GetCursorPos(&position);					//Получаем текущую позицию курсора

		position.x -= m_windowRect.left;
		position.y -= m_windowRect.top;

		if(m_curX == position.x && m_curY == position.y) {
			return;
		}

		m_curX = position.x;
		m_curY = position.y;

		for(auto it = m_listener.begin(); it != m_listener.end(); ++it) {
			if(!(*it)) {
				continue;
			} else if((*it)->mouseMove(MouseEvent(m_curX, m_curY)) == true) {
				return;
			}
		}
	}

	void InputMgr::m_eventMouse(const eMouseKeyCodes code, bool press) {
		for(auto it = m_listener.begin(); it != m_listener.end(); it++) {
			if(!(*it))
				continue;
			//Кнопка нажата
			if(press == true) {
				if((*it)->mousePressed(MouseEventClick(code, m_curX, m_curY)) == true) {
					return;
				}
			} else {								//Кнопка отпущена
				if((*it)->mouseReleased(MouseEventClick(code, m_curX, m_curY)) == true) {
					return;
				}
			}
		}
	}

	void InputMgr::m_mouseWheel(int value) {
		if(m_mouseWheelValue == value) {
			return;
		}

		m_mouseWheelValue = value;
		
		for(auto it = m_listener.begin(); it != m_listener.end(); it++) {
			if(!(*it)) {
				continue;
			} else if((*it)->mouseWheel(MouseEventWheel(m_mouseWheelValue, m_curX, m_curY)) == true) {
				return;
			}
		}
	}

	void InputMgr::m_eventKey(const eKeyCodes keyCode, const wchar_t ch, bool press) {
		for(auto it = m_listener.begin(); it != m_listener.end(); it++) {
			if(!(*it)) {
				continue;
			}
			//Кнопка нажата
			if(press == true) {
				if((*it)->keyPressed(KeyEvent(ch, keyCode)) == true) {
					return;
				} else {							//Кнопка отпущена
					if((*it)->keyReleased(KeyEvent(ch, keyCode)) == true) {
						return;
					}
				}
			}
		}
	}

