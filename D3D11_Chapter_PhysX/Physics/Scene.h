#pragma once

#include "..\DefHeaders.h"
#include "..\Physics\PhysXError.h"
#include "..\Physics\Physics.h"
#include "..\Util\Actor.h"

/*
	Scene определяет поведение отслеживаемых объектов, называемых актерами (Actor), их реакцию на различные события
	Наследуется от PxSimulationEventCallback, чтобы можно было определить реакцию актеров.
	Это достигается переопределением методов PxSimulationEventCallback и передачей указателя на Scene физиксу
*/
class Scene : public PxSimulationEventCallback {
private:
	//Цифры просто взял из мануала PhysX. Позже надо будет все-таки разобраться в последних трех значениях.
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

	std::list<Actor*> m_actorsList;			//Список отслеживаемых актеров. Самое место ему в ObjectManager

	bool isInit;

	PxMaterial* m_planeMaterial;
	PxRigidStatic* m_plane;
public:
	Scene();
	~Scene();

	//Переопределяем методы из PxSimulationEventCallback
	//Вызываются физиксом тогда, когда обнаруживается коллизия или срабатывает триггер
	//TODO: вообще, убрать бы их отсюда в отдельный класс, отвечающий чисто за события из PxSimulationEventCallback
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) {}
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
	virtual void onWake(PxActor** , PxU32 ) {}
	virtual void onSleep(PxActor** , PxU32 ) {}


	bool initScene(Physics* pPhysics);
	PxScene* getPxScene() { if(isInit) return m_pScene; }
	void addActors();				//Тут нужно придумать что-то другое, либо вообще убрать (например, в ObjectManager)

	bool createActors();
	bool createPlane();										//Создает поверхность, "землю", по умолчанию
	bool createPlane(PxVec3 pos, PxMaterial* material);		//Либо можно самому задать ее параметры

	std::wstring getActorPos();				//Вот это надо убрать. Костыль обыкновенный для передачи позиции в Text
	PxVec3 getActorPosVec()		{ return m_actorsList.back()->getActor()->getGlobalPose().p; }	//То же самое для рендера

	void Close();
};