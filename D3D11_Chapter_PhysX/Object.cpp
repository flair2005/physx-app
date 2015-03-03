#include "Object.h"

Object::Object() {
	m_pActor = new Actor();
	m_isAlive = false;
	m_position = PxVec3(0.0f, 0.0f, 0.0f);
	m_velocity = PxVec3(0.0f, 0.0f, 0.0f);
	m_rotation = PxVec3(0.0f, 0.0f, 0.0f);
}

Object::~Object() {

}

void Object::destroy() {
	if(isCreated()) {
		m_pActor->destroy();
	}
	m_vertices.clear();
	m_indices.clear();
	m_pActor = nullptr;
}

bool Object::construct(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 position, PxReal density,
																PxVec3 velocity, PxGeometryHolder geometry) {
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

	if(!m_pActor->attachGeometry(geometry)) {
		Log::get()->err("Actor attachGeometry Failed");
		return false;
	}

	return true;
}

PxActor* Object::getActor() {
	if(isCreated()) {
		return m_pActor->getActor();
	} else {
		Log::get()->err("Ошибка при обращении к Actor: объект не был инициализирован");
		return nullptr;
	}
}

void Object::getModel(std::vector<Vertex> vertices, std::vector<DWORD> indices) {
	m_vertices = vertices;
	m_indices = indices;
}

void Object::updatePosition() {
	//Координаты от физиксовской модели немного отличаются
	m_position.x = m_pActor->getActor()->getGlobalPose().p.x * 2/m_scale.x;
	m_position.y = m_pActor->getActor()->getGlobalPose().p.y * 2/m_scale.y;
	m_position.z = m_pActor->getActor()->getGlobalPose().p.z * -2/m_scale.z;
}