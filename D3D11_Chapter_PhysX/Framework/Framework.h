#pragma once

#include "..\Render\Window.h"
#include "..\Render\Render.h"
#include "..\Util\Log.h"
#include "..\Input\InputMgr.h"
#include "..\Input\InputListener.h"
#include "Stepper.h"
#include "..\Physics\Physics.h"
#include "..\Physics\Scene.h"
#include "..\Util\Fps.h"
#include "..\Util\Text\TextManager.h"

/*
	�������� �����, ���������� �� �������� ���� ��������� �������. � ��� ���������� ���������� ���� �������� �������.
	������ ��� ��������� ������ ������� ������������ ������, ����� (� ������� �������) - ��������� Render,
	������� ������������ ���������� Framework ����� ����� setRender(Render* render).
	����� ����� ������������ ��������������� �������� ������ init(), run() � Close().
	1) init() �������������� ��� ��� ����������; � ������ ������ �������� ���� is_init = true � ����� �������� ����� run()
	2) run() - ����� ���������� ��� ������ ����������. ����������, ������� �����, ������ - ��������� �� �������������, ����
	���� �� �������� ������ � ������ �� Window ����� ����� bool Window::isExit() - true �������� ����������� �������.
	3) ������� ������ � ������, ����� run() ���������� ���� ������ (������� �� �����). ������������ ������ ����������
	����� ������ Close() ����� run(), ����� ��������� ��������� ��������� ������.
*/

class Framework {
protected:
	bool m_frame(float dt);		//�������� �� ������ �������� ����������, ������ ��� ����������, ������ � �.�.
								//dt - �����, ���������� � ���������� ��������
	Window* m_wnd;				//�������� �� ������ ���� � ������������ �������; ��������� ��������� �� Windows
	Render* m_render;			//���, ��� �������� ������ ����������� �� �����. �������� � DirectX11
	InputMgr* m_input;			//�������� �� ��������� ����������������� �����; ������ � Window
	Log* m_log;					//����������� ������ ����������; �������� �� ����� ����� ���� ����� Log::get()
	Stepper* m_stepper;			//������������ ���������. ������������� ����� ����� ����������� � �������
	Physics* m_physics;			//�������� �����, ����������� �������� � PhysX. ������� ���������� � ������� PhysX.
	Scene* m_scene;				//��������� ������������ � ����� ������ ���������� ���������.
	TextManager* m_textManager;	//��� ������ � ������� ����� �� ���� ������. ������� ������, ������ � �������� ��.
	Fps* m_fps;					//��������������� �����; ������� ���������� ������ � ������� � ������ �� �������������� �� ���������

	bool m_init;
	bool m_isRunning;			//true �� ����� ������; false - ���� ��������� ������ � ������ -> ���������� �����������

	float startTime;			//�����, ���������� � ������� ������� ����������
public:
	Framework();
	~Framework();

	bool init();
	void run();
	void Close();

	void preRender(float dt);
	void postRender(float dt);
	
	void setRender(Render* render) { m_render = render; }
	void addInputListener(InputListener* listener);
};
