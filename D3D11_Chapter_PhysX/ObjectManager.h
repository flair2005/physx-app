#pragma once

#include "DefHeaders.h"
#include "Object.h"
#include "Physics.h"
#include "Box.h"

class ObjectManager {
private:
	std::unordered_map<std::string, Object*> objectsMap;
	int m_totalAlive;
public:
	ObjectManager();
	~ObjectManager();

	void destroy();

	bool addBox(std::string key, Physics* pPhysics, PxMaterial* pMaterial, PxVec3 position, PxReal density, PxVec3 velocity, PxVec3 scale);

	bool engageObject(std::string key);

	void updateObjects();

	Vertex* getVertices();
	DWORD* getIndices();
	Object* getObject(std::string key)		{ return objectsMap.at(key);	}

	void getPositions(std::vector<std::pair<PxVec3, int>>& positions);
	void getActors(std::vector<PxActor*>& actors);
	void getObjects(std::vector<Object*>& objects);

	int totalObjects()						{ return objectsMap.size();		}

	int totalVertices();
	int totalIndices();
};