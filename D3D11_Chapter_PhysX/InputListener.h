#pragma once

#include "InputCodes.h"

	//События мыши
	struct MouseEvent {
		MouseEvent(int nx, int ny) : x(nx), y(ny) {}

		//Координаты мыши
		int x;
		int y;
	};

	//События нажатия кнопки мыши
	struct MouseEventClick : public MouseEvent {
		MouseEventClick(eMouseKeyCodes b, int nx, int ny) : MouseEvent(nx, ny), btn(b) {}

		const eMouseKeyCodes btn;			//Клавиша
	};

	//Событие прокрутки мыши
	struct MouseEventWheel : public MouseEvent {
		MouseEventWheel(int nwheel, int nx, int ny) : MouseEvent(nx, ny), wheel(nwheel) {}

		int wheel;
	};

	//Событие клавиши
	struct KeyEvent {
		KeyEvent(wchar_t c, eKeyCodes kc) : wc(c), code(kc) {}

		const wchar_t wc;
		const eKeyCodes code;
	};

	class InputListener {
	public:
		//Если методы возвращают true - событие больше никем не обрабатывается

		//Кнопка нажата
		virtual bool mousePressed(const MouseEventClick& arg) { return false; }
		//Кнопка отпущена
		virtual bool mouseReleased(const MouseEventClick& arg) { return false; }
		//Вращение колесика
		virtual bool mouseWheel(const MouseEventWheel& arg) { return false; }
		//Движение мыши
		virtual bool mouseMove(const MouseEvent& arg) { return false; }

		//Кнопка нажата
		virtual bool keyPressed(const KeyEvent& arg) { return false; }
		//Кнопка отпущена
		virtual bool keyReleased(const KeyEvent& arg) { return false; }
	};

