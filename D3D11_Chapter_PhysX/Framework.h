#pragma once

#include "Window.h"
#include "Render.h"
#include "Log.h"
#include "InputMgr.h"
#include "InputListener.h"
#include "CameraInputListener.h"
#include "Stepper.h"
#include "Physics.h"
#include "Scene.h"
#include "Fps.h"
#include "TextManager.h"
#include "ObjectManager.h"
#include "Terrain.h"
#include "Timer.h"
#include "Input.h"

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
	bool m_frame(double dt);	//�������� �� ������ �������� ����������, ������ ��� ����������, ������ � �.�.
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
	ObjectManager* m_objectManager;	//��������� ����� ���������; � ������� �������� ������ � �������� � ��������, � ����� 
									//������������� ������� (PxRigidBody) ��� �������
	Input* m_newInput;			//TODO REFACTOR

	bool m_init;
	bool m_isRunning;			//true �� ����� ������; false - ���� ��������� ������ � ������ -> ���������� �����������
public:
	Framework();
	~Framework();

	bool init();
	void run();
	void Close();

	void preRender(double dt);
	void postRender(double dt);
	
	void setRender(Render* render) { m_render = render; }
	void addInputListener(InputListener* listener);
};
