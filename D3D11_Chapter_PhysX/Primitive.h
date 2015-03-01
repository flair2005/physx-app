#pragma once

#include "Object.h"

class Primitive : public Object {
private:

public:
	Primitive()				{}
	virtual ~Primitive()	{}

	virtual bool create(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 position, PxReal density,
																PxVec3 velocity);
	virtual PxGeometryHolder createGeometry() = 0;
};