#pragma once

#include "Object.h"
#include "Primitive.h"

class Box : public Primitive {
private:
	void createModel();
public:
	Box(PxVec3 scale);
	virtual ~Box();

	virtual PxGeometryHolder createGeometry();
	virtual bool create(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 position, PxReal density,
															PxVec3 velocity);
};