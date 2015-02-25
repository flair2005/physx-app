#pragma once

#include "..\DefHeaders.h"
#include "..\Render\Render.h"
#include "..\Physics\Physics.h"
#include "..\Physics\Scene.h"

class Stepper {
private:
	Scene* m_pScene;

	PxReal m_accumulator;
	PxReal m_stepSize;
public:
	Stepper();
	~Stepper();

	bool init(Scene* pScene);

	bool advance(float dt);
	void renderDone();

	void Close();
};