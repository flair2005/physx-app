#include "Actor.h"

Actor::Actor() {
	m_isCreated = false;
	m_isAlive = false;
	m_pActor = nullptr;
	m_pShape = nullptr;
	m_pMaterial = nullptr;
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;
	m_velocity.x = 0.0f;
	m_velocity.y = 0.0f;
	m_velocity.z = 0.0f;
	m_width = m_height = m_depth = m_density = 0.0f;
}

Actor::~Actor() {

}

void Actor::destroy() {
	m_pActor->detachShape(*m_pShape);
	m_pMaterial->release();
	m_pActor->release();
	//m_pShape->release();
	m_pActor = nullptr;
	m_pShape = nullptr;
	m_pMaterial = nullptr;
}

bool Actor::constructActor(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 pos, PxReal density, 
							PxVec3 velocity, PxReal width, PxReal height, PxReal depth) {
	if(!pPhysics) {
		Log::get()->err("Actor Create Fail: pPhysics == nullptr");
		return false;
	}
	if(!pMaterial) {
		Log::get()->err("Actor Create Fail: pMaterial == nullptr");
		return false;
	}
	m_pMaterial = pMaterial;
	setPosition(pos);
	if(!setDensity(density)) {
		Log::get()->err("Actor Create Fail: invalid density");
		return false;
	}
	setVelocity(velocity);

	m_width = width;
	m_height = height;
	m_depth = depth;
	
	m_pActor = pPhysics->getPxPhysics()->createRigidDynamic(PxTransform(position));
	//m_pActor = PxCreateDynamic(*(pPhysics->getPxPhysics()), PxTransform(position), PxBoxGeometry(m_width/2, m_height/2, m_depth/2),
		//						*m_pMaterial, density);
	m_pShape = m_pActor->createShape(PxBoxGeometry(m_width/2, m_height/2, m_depth/2), *m_pMaterial);
	PxRigidBodyExt::updateMassAndInertia(*m_pActor, density);
	m_pActor->setLinearVelocity(m_velocity);
	
	m_isCreated = true;
	return true;
}

void Actor::setPosition(PxVec3 pos) {
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;
}

bool Actor::setDensity(PxReal density) {
	if(density > 0.0f && density <= 1.0f) {
		m_density = density;
		return true;
	} else {
		return false;
	}
}

PxVec3 Actor::getPosition() {
	return m_pActor->getGlobalPose().p;
}

