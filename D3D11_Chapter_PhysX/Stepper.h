#pragma once

#include "DefHeaders.h"
#include "Render.h"
#include "Physics.h"
#include "Scene.h"


/*
	Пока что все, что Stepper делает - вызывает метод PxScene::simulate() 
*/
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