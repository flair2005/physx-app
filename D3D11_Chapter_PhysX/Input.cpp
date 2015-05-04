#include "Input.h"

Input::Input() {
	DIKeyboard = nullptr;
	DIMouse = nullptr;
	m_isInited = false;
}

bool Input::init(HINSTANCE hInstance, HWND hwnd) {
	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, NULL);
	if(FAILED(hr)) {
		Log::get()->err("Input::init() failed: unable to create DirectInput Interface");
		return false;
	}

	hr = DirectInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, NULL);
	if(FAILED(hr)) {
		Log::get()->err("Input::init() failed: unable to create SysKeyboard device");
		return false;
	}

	hr = DirectInput->CreateDevice(GUID_SysMouse, &DIMouse, NULL);
	if(FAILED(hr)) {
		Log::get()->err("Input::init() failed: unable to create SysMouse device");
		return false;
	}

	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr)) {
		Log::get()->err("Input::init() failed: unable to set SysKeyboard data format");
		return false;
	}
	hr = DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(hr)) {
		Log::get()->err("Input::init() failed: unable to set SysKeyboard cooperative level");
		return false;
	}

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	if(FAILED(hr)) {
		Log::get()->err("Input::init() failed: unable to set SysMouse data format");
		return false;
	}
	hr = DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	if(FAILED(hr)) {
		Log::get()->err("Input::init() failed: unable to set SysMouse cooperative level");
		return false;
	}

	m_isInited = true;
	return true;
}

void Input::detectInput(double dt, Camera* pCamera) {
	if(!m_isInited) {
		return;
	}

	DIMOUSESTATE mouseCurrState;
	BYTE keyboardState[256];

	HRESULT hr = DIKeyboard->Acquire();
	if(FAILED(hr)) {
		return;
	}
	hr = DIMouse->Acquire();
	if(FAILED(hr)) {
		return;
	}

	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	if(keyboardState[DIK_ESCAPE] & 0x80) {
		//	TODO FIX
		PostQuitMessage(0);
	}

	if(keyboardState[DIK_A] & 0x80)	{
		pCamera->moveLeft(dt);
	}
	if(keyboardState[DIK_D] & 0x80)	{
		pCamera->moveRight(dt);
	}
	if(keyboardState[DIK_W] & 0x80)	{
		pCamera->moveForward(dt);
	}
	if(keyboardState[DIK_S] & 0x80)	{
		pCamera->moveBack(dt);
	}
	if((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
	{
		float camYaw = mouseCurrState.lX * 0.001f;
		float camPitch = mouseCurrState.lY * 0.001f;
		pCamera->rotate(camYaw, camPitch);
		mouseLastState = mouseCurrState;
	}
}

void Input::Close() {
	DIKeyboard->Unacquire();
	DIMouse->Unacquire();
	_RELEASE(DIKeyboard);
	_RELEASE(DIMouse);
}