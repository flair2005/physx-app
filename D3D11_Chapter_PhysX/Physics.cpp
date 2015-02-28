#include "Physics.h"

Physics::Physics() {
	m_pPhysX = nullptr;
	m_pFoundation = nullptr;
	m_pError = nullptr;
	m_pProfileZoneManager = nullptr;
	m_pCooking = nullptr;
	isInit = false;
}

Physics::~Physics() {

}

bool Physics::init(Render* pRender) {
	if(!pRender) {
		m_pError->reportError(PxErrorCode::eINVALID_PARAMETER, "Пустой параметр pRender* - Physics::init(Render* pRender)",
								__FILE__, __LINE__);
		return false;
	}
	m_pRender = pRender;

	//Создание Foundation
	m_pError = new PhysXError();			//Почему не использовать Log::get()? PhysXError используется как callback самим физиксом			
	m_pAllocator = new PxDefaultAllocator();
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_pAllocator, *m_pError);
	if(!m_pFoundation) {
		m_pError->reportError(PxErrorCode::eABORT, "Foundation Create Fail", __FILE__, __LINE__);
		return false;
	}
	
	m_pProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(m_pFoundation);
	if(!m_pProfileZoneManager) {
		m_pError->reportError(PxErrorCode::eABORT, "ProfileZoneManager Create Fail", __FILE__, __LINE__);
		return false;
	}

	//Запускаем PhysX
	bool recordMemoryAllocations = true;				//Флаг, обозначающий, используется ли ProfileZoneManager
	m_pPhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), recordMemoryAllocations, m_pProfileZoneManager);
	if(!m_pPhysX) {
		m_pError->reportError(PxErrorCode::eABORT, "PhysX Create Fail", __FILE__, __LINE__);
		return false;
	}

	//запускаем Cooking
	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, PxCookingParams(PxTolerancesScale()));
	if(!m_pCooking) {
		m_pError->reportError(PxErrorCode::eABORT, "Cooking Create Fail", __FILE__, __LINE__);
		return false;
	}

	//запускаем Расширения
	if(!PxInitExtensions(*m_pPhysX)) {
		m_pError->reportError(PxErrorCode::eABORT, "Extentions Create Fail", __FILE__, __LINE__);
		return false;
	}

	//Унифицированное с полем (картой?) высот определение коллизий
	PxRegisterUnifiedHeightFields(*m_pPhysX);

	isInit = true;
	return true;
}

void Physics::Close() {
	//Сперва освобождаются зависимые от Foundation ресурсы
	m_pError = nullptr;

	if(m_pPhysX) {
		m_pPhysX->release();
	}
	if(m_pCooking) {
		m_pCooking->release();
	}

	if(m_pProfileZoneManager) {
		m_pProfileZoneManager->release();
	}

	m_pAllocator = nullptr;

	if(m_pFoundation) {				//Но почему-то не все освобождается и Foundation ругается на это. FIXME!
		m_pFoundation->release();
	}
	
	m_pPhysX = nullptr;
	m_pProfileZoneManager = nullptr;
	m_pCooking = nullptr;
	m_pFoundation = nullptr;

	Log::get()->debug("PhysX Close");
}