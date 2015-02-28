#pragma once

#include "Object.h"
#include "Primitive.h"

class Box : public Primitive {
private:
	PxReal m_width;
	PxReal m_height;
	PxReal m_depth;

	void createModel();
public:
	Box(PxReal width, PxReal height, PxReal depth);
	Box(PxVec3 size);
	virtual ~Box();

	virtual PxGeometry createGeometry();
	virtual bool create(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 position, PxReal density,
															PxVec3 velocity);
};