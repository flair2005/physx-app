#include "Object.h"

Object::Object() {
	m_pActor = new Actor();
	m_isAlive = false;
	m_position = PxVec3(0.0f, 0.0f, 0.0f);
	m_velocity = PxVec3(0.0f, 0.0f, 0.0f);
}

Object::~Object() {

}

void Object::destroy() {
	if(isCreated()) {
		m_pActor->destroy();
	}
	m_pActor = nullptr;
}

bool Object::construct(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 position, PxReal density,
																PxVec3 velocity, PxGeometry geometry) {
	m_position = position;
	m_velocity = velocity;

	if(!pPhysics) {
		Log::get()->err("Actor Create Fail: pPhysics == nullptr");
		return false;
	}
	if(!pMaterial) {
		Log::get()->err("Actor Create Fail: pMaterial == nullptr");
		return false;
	}

	if(!m_pActor->construct(pPhysics, pMaterial, m_position, density, m_velocity)) {
		Log::get()->err("Actor Construction Failed");
		return false;
	}

	if(!m_pActor->attachGeometry(&geometry)) {
		Log::get()->err("Actor attachGeometry Failed");
		return false;
	}

	return true;
}

PxRigidDynamic* Object::getActor() const {
	if(isCreated()) {
		return m_pActor->getRigidDynamic();
	} else {
		Log::get()->err("Ошибка при обращении к Actor: объект не был инициализирован");
		return nullptr;
	}
}

void Object::getModel(std::vector<Vertex> vertices, std::vector<DWORD> indices) {
	m_vertices = vertices;
	m_indices = indices;
}

/*bool Object::attachGeometry(PxGeometry* geometry) {
	if(!m_pActor->attachGeometry(geometry)) {
		return false;
	}

	return true;
}*/