#pragma once

#include "DefHeaders.h"
#include "Object.h"
#include "Physics.h"
#include "Box.h"

class ObjectManager {
private:
	std::unordered_map<std::string, Object*> objectsMap;
public:
	ObjectManager();
	~ObjectManager();

	bool addBox(std::string key, Physics* pPhysics, PxMaterial* pMaterial, PxVec3 position, PxReal density, PxVec3 velocity);

	bool engageObject(std::string key);

	Vertex* getVertices();
	DWORD* getIndices();

	int totalVertices();
	int totalIndices();
};