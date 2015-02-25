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
	bool m_isAlive;
	PxVec3 position;

	//��������� ������� - ������ ��� �����
	//TODO �������� ����� �������
	PxReal m_width;						//������
	PxReal m_height;					//������
	PxReal m_depth;						//�������
	PxReal m_density;					//���������
	PxVec3 m_velocity;					//��������

	
public:
	Actor();
	~Actor();

	bool constructActor(Physics* pPhysics, PxMaterial* pMaterial, PxVec3 pos, PxReal density, PxVec3 velocity,
							PxReal width, PxReal height, PxReal depth);
	void destroy();

	PxRigidDynamic* getActor()			{ if(m_isCreated) return m_pActor;		}
	PxShape* getShape()					{ if(m_isCreated) return m_pShape;		}
	PxMaterial* getMaterial()			{ if(m_isCreated) return m_pMaterial;	}
	PxVec3 getPosition();

	bool isAlive()						{ if(m_isCreated) return m_isAlive;		}
	
	void engage()						{ if(m_isCreated) m_isAlive = true;		}
	void kill()							{ if(m_isCreated) m_isAlive = false;	}

	void setPosition(PxVec3 pos);
	bool setDensity(PxReal density);
	void setVelocity(PxVec3 velocity)	{ m_velocity = velocity; }
};