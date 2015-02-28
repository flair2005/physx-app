#pragma once

#include "DefHeaders.h"
#include "PhysXError.h"
#include "Physics.h"
#include "Object.h"
#include "Box.h"
/*
	Scene ���������� ��������� ������������� ��������, ���������� �������� (Actor), �� ������� �� ��������� �������
	����������� �� PxSimulationEventCallback, ����� ����� ���� ���������� ������� �������.
	��� ����������� ���������������� ������� PxSimulationEventCallback � ��������� ��������� �� Scene �������
*/
class Scene : public PxSimulationEventCallback {
private:
	//����� ������ ���� �� ������� PhysX. ����� ���� ����� ���-���� ����������� � ��������� ���� ���������.
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

	std::list<Object*> m_actorsList;			//������ ������������� �������. ����� ����� ��� � ObjectManager

	bool isInit;

	PxMaterial* m_planeMaterial;
	PxRigidStatic* m_plane;
public:
	Scene();
	~Scene();

	//�������������� ������ �� PxSimulationEventCallback
	//���������� �������� �����, ����� �������������� �������� ��� ����������� �������
	//TODO: ������, ������ �� �� ������ � ��������� �����, ���������� ����� �� ������� �� PxSimulationEventCallback
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) {}
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
	virtual void onWake(PxActor** , PxU32 ) {}
	virtual void onSleep(PxActor** , PxU32 ) {}


	bool initScene(Physics* pPhysics);
	PxScene* getPxScene() { if(isInit) return m_pScene; }
	void addActors();				//��� ����� ��������� ���-�� ������, ���� ������ ������ (��������, � ObjectManager)

	bool createActors();
	bool createPlane();										//������� �����������, "�����", �� ���������
	bool createPlane(PxVec3 pos, PxMaterial* material);		//���� ����� ������ ������ �� ���������

	std::wstring getActorPos();				//��� ��� ���� ������. ������� ������������ ��� �������� ������� � Text
	//PxVec3 getActorPosVec()		{ return m_actorsList.back()->getActor()->getGlobalPose().p; }	//�� �� ����� ��� �������

	void Close();
};