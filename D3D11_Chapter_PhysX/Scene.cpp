#include "Scene.h"
#include "SimulationEvent.h"

Scene::Scene() {
	m_pPhysics = nullptr;
	m_pError = nullptr;
	m_pScene = nullptr;
	m_pCpuDispatcher = nullptr;
	m_pCudaContextManager = nullptr;
	m_planeMaterial = nullptr;		//Убрать!
	m_pSimulationEvent = nullptr;
	isInit = false;
}

Scene::~Scene() {

}

bool Scene::initScene(Physics* pPhysics) {
	m_pSimulationEvent = new SimulationEvent();
	m_pPhysics = pPhysics;
	
	PxSceneDesc sceneDesc(m_pPhysics->getPxPhysics()->getTolerancesScale());

	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);		//Ставим гравитацию на сцене
	customizeSceneDesc(sceneDesc);

	if(!sceneDesc.cpuDispatcher) {
		m_pCpuDispatcher = PxDefaultCpuDispatcherCreate(m_NbThreads);
		if(!m_pCpuDispatcher) {
			m_pError->reportError(PxErrorCode::eABORT, "CPU Dispatcher Create Fail", __FILE__, __LINE__);
			return false;
		}
		sceneDesc.cpuDispatcher = m_pCpuDispatcher;
	}

	if(!sceneDesc.filterShader) {
		sceneDesc.filterShader = &PxDefaultSimulationFilterShader;
	}
	
	PxCudaContextManagerDesc contextManagerDesc;
	contextManagerDesc.ctx = NULL;
	contextManagerDesc.graphicsDevice = m_pPhysics->getRender()->getDevice();
	contextManagerDesc.interopMode = PxCudaInteropMode::D3D11_INTEROP;
	contextManagerDesc.maxMemorySize[m_maxMemorySize];
	contextManagerDesc.memoryBaseSize[m_memoryBaseSize];
	contextManagerDesc.memoryPageSize[m_memoryBaseSize];
	m_pCudaContextManager = PxCreateCudaContextManager(*(m_pPhysics->getFoundation()), contextManagerDesc, m_pPhysics->getProfileZoneManager());
	if(!m_pCudaContextManager) {
		m_pError->reportError(PxErrorCode::eABORT, "CudaContextManager Create Fail", __FILE__, __LINE__);
		return false;
	}

#ifdef PX_WINDOWS
	if(!sceneDesc.gpuDispatcher && m_pCudaContextManager)
	{
		sceneDesc.gpuDispatcher = m_pCudaContextManager->getGpuDispatcher();
	}
#endif

	m_pScene = m_pPhysics->getPxPhysics()->createScene(sceneDesc);
	if(!m_pScene) {
		m_pError->reportError(PxErrorCode::eABORT, "Scene Create Fail", __FILE__, __LINE__);
		return false;
	}
	
	isInit = true;
	return true;
}

void Scene::customizeSceneDesc(PxSceneDesc& sceneDesc) {
	sceneDesc.filterShader = defaultFilterShader;
	sceneDesc.simulationEventCallback = m_pSimulationEvent;		//Говорим физиксу, что в этом классе находятся методы, нужные
															//для определения поведения при событиях (коллизия, триггер)
}

PxFilterFlags Scene::defaultFilterShader(	
	PxFilterObjectAttributes attributes0, PxFilterData filterData0, 
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize) {
	// let triggers through
	if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1)) {
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where 
	// the filtermask of A contains the ID of B and vice versa.
//	if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;
}

void Scene::Close() {
	if(m_pScene) {
		m_pScene->release();
	}

	if(m_pCudaContextManager) {
		m_pCudaContextManager->release();
	}

	if(m_planeMaterial) {
		m_planeMaterial->release();
	}

	delete m_pCpuDispatcher;

	m_pCudaContextManager = nullptr;
	m_pScene = nullptr;

	Log::get()->debug("Scene Close");
}

//Создаем Plane по умолчанию
//В этом случае просто вызываем отсюда createPlane с параметрами
//TODO: plane является таким же объектом, отправляется в ObjectManager, прикрепляется геометрия типа PxPlaneGeometry
//Потом, собственно, рендерится
bool Scene::createPlane() {
	//Опытным путем было установлено, что возможны только два значения для Plane:
	//(1.0f; 0.0f; 0.0f) и (0.0f; 1.0f; 0.0f)
	PxVec3 pos;
	pos.x = 0.0f;
	pos.y = 1.0f;
	pos.z = 0.0f;
	m_planeMaterial = m_pPhysics->getPxPhysics()->createMaterial(0.5f, 0.5f, 0.1f);
	if(!m_planeMaterial) {
		m_pError->reportError(PxErrorCode::eABORT, "Plane Material Create Fail", __FILE__, __LINE__);
		return false;
	}
	if(!createPlane(pos, m_planeMaterial)) {
		return false;
	}

	return true;
}

bool Scene::createPlane(PxVec3 pos, PxMaterial* material) {
	m_planeMaterial = material;
	m_plane = PxCreatePlane(*(m_pPhysics->getPxPhysics()), PxPlane(pos, 0), *material);
	if(!m_plane) {
		return false;
	}

	return true;
}

//Отдаем актеров из списка сцене, чтобы она могла ими управлять
void Scene::addActors(ObjectManager* objectManager) {
	std::vector<PxActor*> actors;
	objectManager->getActors(actors);
	for(int i = 0; i < actors.size(); i++) {
		m_pScene->addActor(*actors[i]);
	}

	m_pScene->addActor(*m_plane);
}