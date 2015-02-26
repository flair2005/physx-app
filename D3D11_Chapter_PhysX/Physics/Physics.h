#pragma once

#include "..\DefHeaders.h"
#include "..\Physics\PhysXError.h"
#include "..\Render\Render.h"

using namespace physx;

class Physics {
private:
	PxFoundation* m_pFoundation;	//Основа для создания объектов PhysX
	PxPhysics* m_pPhysX;
	PxErrorCallback* m_pError;	//Используется для оповещения об ошибках, 
								//принимает код ошибки из структуры PxErrorCode, сообщение, имя файла (__FILE) и строку (__LINE__).
	Render* m_pRender;
	PxDefaultAllocator* m_pAllocator;
	PxProfileZoneManager* m_pProfileZoneManager;
	PxCooking* m_pCooking;
	bool isInit;
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
