#include "Stepper.h"

Stepper::Stepper() {
	m_pScene = nullptr;
	m_accumulator = 0.0f;
	m_stepSize = 1.0f / 60.0f;
}

Stepper::~Stepper() {

}

bool Stepper::advance(float dt) {
	m_accumulator += dt;
	if(m_accumulator < m_stepSize) {
		return false;
	}

	m_accumulator -= m_stepSize;

	m_pScene->getPxScene()->simulate(m_stepSize);
	
	return true;
}

bool Stepper::init(Scene* pScene) {
	if(!pScene) {
		Log::get()->err("Stepper Init Fail: pScene == nullptr");
		return false;
	}
	m_pScene = pScene;
	
	return true;
}

void Stepper::Close() {
	Log::get()->debug("Stepper Close");
}