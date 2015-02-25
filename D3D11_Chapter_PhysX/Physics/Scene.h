#pragma once

#include "..\DefHeaders.h"
#include "..\Physics\PhysXError.h"
#include "..\Physics\Physics.h"
#include "..\Util\Actor.h"

class Scene : public PxSimulationEventCallback {
private:
	static const PxU32 m_NbThreads = 1;
	static const PxU32 m_maxMemorySize = 536870912;
	static const PxU32 m_memoryBaseSize = 134217728;
	static const PxU32 m_memoryPageSize = 1048576;

	Physics* m_pPhysics;
	PxScene* m_pScene;
	PxErrorCallback* m_pError;
	PxCpuDispatcher* m_pCpuDispatcher;
	PxCudaContextManager* m_pCudaContextManager;

	void customizeSceneDesc(PxSceneDesc& sceneDesc);

	static PxFilterFlags defaultFilterShader(	
	PxFilterObjectAttributes attributes0, PxFilterData filterData0, 
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);

	std::list<Actor*> m_actorsList;

	bool isInit;

	PxMaterial* m_planeMaterial;
	PxRigidStatic* m_plane;
public:
	Scene();
	~Scene();

	//Переопределяем методы из PxSimulationEventCallback
	//Вызываются физиксом тогда, когда обнаруживается коллизия или срабатывает триггер
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) {}
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
	virtual void onWake(PxActor** , PxU32 ) {}
	virtual void onSleep(PxActor** , PxU32 ) {}


	bool initScene(Physics* pPhysics);
	PxScene* getPxScene() { if(isInit) return m_pScene; }
	void addActors();

	bool createActors();
	bool createPlane();
	bool createPlane(PxVec3 pos, PxMaterial* material);

	std::wstring getActorPos();
	PxVec3 getActorPosVec()		{ return m_actorsList.back()->getActor()->getGlobalPose().p; }

	void Close();
};