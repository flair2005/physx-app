#pragma once

#include "Window.h"
#include "Render.h"
#include "Log.h"
#include "InputMgr.h"
#include "InputListener.h"
#include "Stepper.h"
#include "Physics.h"
#include "Scene.h"
#include "Fps.h"
#include "TextManager.h"

class Framework {
protected:
	bool m_frame(float dt);

	Window* m_wnd;
	Render* m_render;
	InputMgr* m_input;
	Log* m_log;
	Stepper* m_stepper;
	Physics* m_physics;
	Scene* m_scene;
	TextManager* m_textManager;
	Fps* m_fps;

	bool m_init;
	bool m_isRunning;

	float startTime;
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
