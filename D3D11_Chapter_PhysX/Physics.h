#pragma once

#include "DefHeaders.h"
#include "PhysXError.h"
#include "Render.h"

using namespace physx;

class Physics {
private:
	PxFoundation* m_pFoundation;	//������ ��� �������� �������� PhysX
	PxPhysics* m_pPhysX;
	PxErrorCallback* m_pError;	//������������ ��� ���������� �� �������, 
								//��������� ��� ������ �� ��������� PxErrorCode, ���������, ��� ����� (__FILE) � ������ (__LINE__).
	Render* m_pRender;
	PxDefaultAllocator* m_pAllocator;
	PxProfileZoneManager* m_pProfileZoneManager;
	PxCooking* m_pCooking;
	bool isInit;

	bool initVisualDebugger();
public:
	Physics();
	~Physics();

	bool init(Render* pRender);

	PxPhysics* getPxPhysics()							{ if(isInit) return m_pPhysX;				}
	PxProfileZoneManager* getProfileZoneManager()		{ if(isInit) return m_pProfileZoneManager;	}
	PxFoundation* getFoundation()						{ if(isInit) return m_pFoundation;			}
	PxErrorCallback* getErrorCallback()					{ if(isInit) return m_pError;				}
	Render* getRender()									{ if(isInit) return m_pRender;				}						

	void Close();
};
