#include "Primitive.h"

bool Primitive::create(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 position, PxReal density,
															PxVec3 velocity) {
	if(!Object::isCreated()) {
		Object::construct(pPhysics, pMaterial, position, density, velocity, createGeometry());
		return true;
	}
}