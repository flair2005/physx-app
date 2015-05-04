#pragma once

#include "InputCodes.h"

	//������� ����
	struct MouseEvent {
		MouseEvent(int nx, int ny) : x(nx), y(ny) {}

		//���������� ����
		int x;
		int y;
	};

	//������� ������� ������ ����
	struct MouseEventClick : public MouseEvent {
		MouseEventClick(eMouseKeyCodes b, int nx, int ny) : MouseEvent(nx, ny), btn(b) {}

		const eMouseKeyCodes btn;			//�������
	};

	//������� ��������� ����
	struct MouseEventWheel : public MouseEvent {
		MouseEventWheel(int nwheel, int nx, int ny) : MouseEvent(nx, ny), wheel(nwheel) {}

		int wheel;
	};

	//������� �������
	struct KeyEvent {
		KeyEvent(wchar_t c, eKeyCodes kc) : wc(c), code(kc) {}

		const wchar_t wc;
		const eKeyCodes code;
	};

	class InputListener {
	public:
		//���� ������ ���������� true - ������� ������ ����� �� ��������������

		//������ ������
		virtual bool mousePressed(const MouseEventClick& arg) { return false; }
		//������ ��������
		virtual bool mouseReleased(const MouseEventClick& arg) { return false; }
		//�������� ��������
		virtual bool mouseWheel(const MouseEventWheel& arg) { return false; }
		//�������� ����
		virtual bool mouseMove(const MouseEvent& arg) { return false; }

		//������ ������
		virtual bool keyPressed(const KeyEvent& arg) { return false; }
		//������ ��������
		virtual bool keyReleased(const KeyEvent& arg) { return false; }
	};

