#pragma once

#include "DefHeaders.h"
#include "PhysXError.h"
#include "Physics.h"
#include "Object.h"
#include "ObjectManager.h"

class SimulationEvent;
/*
	Scene ���������� ��������� ������������� ��������, ���������� �������� (Actor), �� ������� �� ��������� �������
	����������� �� PxSimulationEventCallback, ����� ����� ���� ���������� ������� �������.
	��� ����������� ���������������� ������� PxSimulationEventCallback � ��������� ��������� �� Scene �������
*/
class Scene {
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
	SimulationEvent* m_pSimulationEvent;

	void customizeSceneDesc(PxSceneDesc& sceneDesc);

	static PxFilterFlags defaultFilterShader(	
	PxFilterObjectAttributes attributes0, PxFilterData filterData0, 
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);

	bool isInit;

	PxMaterial* m_planeMaterial;	//������!
	PxRigidStatic* m_plane;			//������!
public:
	Scene();
	~Scene();

	bool initScene(Physics* pPhysics);
	PxScene* getPxScene() { if(isInit) return m_pScene; }
	void addActors(ObjectManager* objectManager);

	bool createPlane();										//������� �����������, "�����", �� ���������, ������ �� ������
	bool createPlane(PxVec3 pos, PxMaterial* material);		//���� ����� ������ ������ �� ��������

	void Close();
};