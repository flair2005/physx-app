#pragma once

#include <dinput.h>

#include "DefHeaders.h"
#include "macros.h"
#include "Log.h"

#include "Camera.h"

class Input {
private:
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;

	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DirectInput;

	bool m_isInited;
public:
	Input();
	~Input() {}

	bool init(HINSTANCE hInstance, HWND hwnd);
	//	TODO REFACTOR: 
	//	- убрать костыль (Camera*); 
	//	- Input работает по типу InputMgr и реализует observer с subject;
	//	- перепилить все структуры в InputListener под Input;
	//	- переименовать Input?
	//	- WinAPI больше не используется для обработки ввода
	void detectInput(double dt, Camera* pCamera);

	void Close();
};

/*
	DIK_A ... DIK_Z					A through Z
	DIK_0 ... DIK_9					0 through 9
	DIK_F1 ... DIK_F15				F1 through F15
	DIK_NUMPAD0 ... DIK_NUMPAD9		0 through 9 on the numpad. The keys are the same whether or not num lock is turned on.
	DIK_ESCAPE						Escape key
	DIK_SPACE						Spacebar
	DIK_RETURN						Return or Enter key
	DIK_NUMPADENTER					Enter key on numpad
	DIK_UP							Up arrow key
	DIK_DOWN						Down arrow key
	DIK_LEFT						Left arrow key
	DIK_RIGHT						Right arrow key
	DIK_SUBTRACT					- key on keypad
	DIK_ADD							+ key on keypad
	DIK_MINUS						- key next to the equals key on top row
	DIK_EQUALS						= key next to backspace
	DIK_BACK						backspace key
	DIK_TAB							Tab key
	DIK_LCONTROL					Left side Ctrl key
	DIK_LSHIFT						Left Shift key
	DIK_RSHIFT						Right side Shift key
	DIK_LMENU						Alt key on the left
	DIK_RMENU						Alt key on the right
	DIK_RCONTROL					Right side control key
	DIK_LWIN						Left side windows key
*/