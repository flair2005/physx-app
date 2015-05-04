/*#pragma once

#include "DefHeaders.h"

using namespace physx;

namespace Object {

	class BaseObject : public PxRigidDynamic {
	private:
		PxActor* m_pActor;
	public:
		RigidDynamicObject();
		virtual ~RigidDynamicObject();

		bool create(PxPhysics* pPhysics, PxTransform* position, PxGeometry* geometry, PxReal density);
	};

}*/