#include "Actor.h"

Actor::Actor() {
	m_isCreated = false;
	m_pActor = nullptr;
	m_pShape = nullptr;
	m_pMaterial = nullptr;
	m_density = 0.0f;
}

Actor::~Actor() {

}

void Actor::destroy() {
	m_pActor->detachShape(*m_pShape);
	m_pMaterial->release();
	m_pActor->release();
	m_pActor = nullptr;
	m_pShape = nullptr;
	m_pMaterial = nullptr;
}


bool Actor::construct(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 pos, PxReal density, 
							PxVec3 velocity) {
	if(m_isCreated) {
		Log::get()->err("Actor уже был создан");
		return false;
	}
	
	m_pMaterial = pMaterial;

	if(density > 0.0f && density <= 1.0f) {
		m_density = density;
	} else {
		Log::get()->err("Invalid Density Value");
		return false;
	}
	
	m_pActor = pPhysics->getPxPhysics()->createRigidDynamic(PxTransform(pos));

	PxRigidBodyExt::updateMassAndInertia(*m_pActor, density);
	m_pActor->setLinearVelocity(velocity);
	
	return true;
}

bool Actor::attachGeometry(PxGeometry* geometry) {
	if(geometry) {
		m_pShape = m_pActor->createShape(*geometry, *m_pMaterial);
		m_isCreated = true;
		return true;
	} else {
		Log::get()->err("Invalid Geometry");
		return false;
	}
}