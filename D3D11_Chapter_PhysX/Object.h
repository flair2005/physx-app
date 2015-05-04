#pragma once

#include "DefHeaders.h"
#include "Physics.h"
#include "Actor.h"
#include "DataStructures.h"

class  Object {
private:
	Actor* m_pActor;
	bool m_isAlive;

	std::vector<Vertex> m_vertices;
	std::vector<DWORD> m_indices;
protected:
	PxVec3 m_position;
	PxVec3 m_velocity;
	PxVec3 m_scale;

	void getModel(std::vector<Vertex> vertices,  std::vector<DWORD> indices);
	bool construct(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 position, PxReal density, PxVec3 velocity, PxGeometryHolder geometry);
public:
	Object();
	virtual ~Object();

	virtual bool create(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 position, PxReal density,
																PxVec3 velocity) = 0;
	virtual void destroy();

	PxVec3 getPosition()					{ return m_position;			}
	PxVec3 getScale()						{ return m_scale;				}
	PxQuat getRotation()					{ return m_pActor->getActor()->getGlobalPose().q;			}
	void updatePosition();

	std::vector<Vertex> getVertices()		{ return m_vertices;			}
	std::vector<DWORD> getIndices()			{ return m_indices;				}
	
	void setPosition(PxVec3 position)		{ m_position = position;		}
	void setVelocity(PxVec3 velocity)		{ m_velocity = velocity;		}

	bool isAlive()							{ return m_isAlive;				}
	bool isCreated() const					{ return m_pActor->isCreated(); }

	void engage()							{ m_isAlive = true;				}
	void kill()								{ m_isAlive = false;			}

	XMMATRIX convertMatrix(PxQuat q);

	PxActor* getActor();
};