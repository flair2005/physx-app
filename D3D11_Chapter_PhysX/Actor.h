#pragma once

#include "DefHeaders.h"
#include "Physics.h"

using namespace physx;

class Actor {
private:
	PxRigidDynamic* m_pActor;
	PxShape* m_pShape;
	PxMaterial* m_pMaterial;
	bool m_isCreated;
	PxReal m_density;
public:
	Actor();
	~Actor();

	bool construct(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 pos, PxReal density, PxVec3 velocity);
	bool attachGeometry(PxGeometry* geometry);

	void destroy();

	PxRigidDynamic* getActor()				{ return m_pActor;		}

	bool isCreated()						{ return m_isCreated;	}
};