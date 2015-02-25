#include "Scene.h"

Scene::Scene() {
	m_pPhysics = nullptr;
	m_pError = nullptr;
	m_pScene = nullptr;
	m_pCpuDispatcher = nullptr;
	m_pCudaContextManager = nullptr;
	m_planeMaterial = nullptr;
	isInit = false;
}

Scene::~Scene() {

}
//PxPhysics* pPhysX, PxFoundation* pFoundation, PxProfileZoneManager* pProfileZoneManager, 
//PxErrorCallback* pError, ID3D11Device* pd3dDevice
bool Scene::initScene(Physics* pPhysics) {
	m_pPhysics = pPhysics;
	
	PxSceneDesc sceneDesc(m_pPhysics->getPxPhysics()->getTolerancesScale());

	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
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
	sceneDesc.simulationEventCallback = this;
}

PxFilterFlags Scene::defaultFilterShader(	
	PxFilterObjectAttributes attributes0, PxFilterData filterData0, 
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where 
	// the filtermask of A contains the ID of B and vice versa.
	if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;
}

void Scene::Close() {
	for(int i = 0; i < m_actorsList.size(); i++) {
		m_actorsList.front()->destroy();
	}
	m_actorsList.clear();
	m_pScene->release();
	m_pCudaContextManager->release();
	m_pCpuDispatcher = nullptr;
	m_pCudaContextManager = nullptr;
	m_pScene = nullptr;
	Log::get()->debug("Scene Close");
}

void Scene::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
	for(PxU32 i=0; i < nbPairs; i++) {
		const PxContactPair& cp = pairs[i];

		if(cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//if((pairHeader.actors[0] == mSubmarineActor) || (pairHeader.actors[1] == mSubmarineActor))
			{
				//PxActor* otherActor = (mSubmarineActor == pairHeader.actors[0]) ? pairHeader.actors[1] : pairHeader.actors[0];			
				//Seamine* mine =  reinterpret_cast<Seamine*>(otherActor->userData);
				// insert only once
				//if(std::find(mMinesToExplode.begin(), mMinesToExplode.end(), mine) == mMinesToExplode.end())
				//	mMinesToExplode.push_back(mine);
				Log::get()->debug("COLLIDE");

				break;
			}
		}
	}
}

bool Scene::createActors() {
	PxVec3 position;
	position.x = 5.0f;
	position.y = 5.1f;
	position.z = 2.0f;
	PxVec3 velocity;
	velocity.x = 0.01f;
	velocity.y = 0.0f;
	velocity.z = 0.0f;
	Actor* simpleActor = new Actor();
	simpleActor->constructActor(m_pPhysics, m_pPhysics->getPxPhysics()->createMaterial(0.5f, 0.5f, 0.1f), 
									position, 1.0f, velocity, 10.2f, 10.2f, 10.2f);
	simpleActor->engage();
	m_actorsList.push_front(simpleActor);

	return true;
}

bool Scene::createPlane() {
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

void Scene::addActors() {
	m_pScene->addActor(*m_plane);

	
	if(m_actorsList.size() == 1) {
		m_pScene->addActor(*(m_actorsList.front()->getActor()));
		Log::get()->debug("(x: %f, y: %f, z: %f)", m_actorsList.front()->getPosition().x, m_actorsList.front()->getPosition().y,
													m_actorsList.front()->getPosition().z);
		return;
	}

	Actor* pr;
	for(pr = m_actorsList.front(); pr != m_actorsList.back(); pr++) {
		m_pScene->addActor(*(pr->getActor()));
		Log::get()->debug("(x: %f, y: %f, z: %f)", m_actorsList.front()->getPosition().x, m_actorsList.front()->getPosition().y,
													m_actorsList.front()->getPosition().z);
		
	}
}

std::wstring Scene::getActorPos() {
	
	wchar_t tmp[255];

	swprintf(tmp, 255, L"(%f; %f; %f)", m_actorsList.front()->getPosition().x, m_actorsList.back()->getPosition().y,
					m_actorsList.back()->getPosition().z);
	return tmp;
}