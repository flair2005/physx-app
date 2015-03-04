#include "ObjectManager.h"

ObjectManager::ObjectManager() {
	objectsMap.clear();
}

ObjectManager::~ObjectManager() {
	for(int i = 0; i < objectsMap.size(); i++) {
		objectsMap.end()->second->destroy();
		objectsMap.end()->second = nullptr;
	}
}

bool ObjectManager::addBox(std::string key, Physics* pPhysics, PxMaterial* pMaterial, PxVec3 position,
																		PxReal density, PxVec3 velocity, PxVec3 scale) {
	Box* box = new Box(scale);
	if(!box->create(pPhysics, pMaterial, position, density, velocity)) {
		Log::get()->err("Box Create Failed");
		return false;
	}
	box->getActor()->setName(key.c_str());
	objectsMap[key] = box;

	return true;
}

bool ObjectManager::engageObject(std::string key) {
	if(objectsMap.count(key) == 0) {
		Log::get()->err("Engage Object Failed: Invalid Key");
		return false;
	}

	objectsMap.find(key)->second->engage();
}

Vertex* ObjectManager::getVertices() {
	int totalV = 0;
	std::vector<Vertex> vertices;
	//Итератор бегает по списку объектов, цикл прекращается, когда достигнут последний элемент
	//По итератору идет обращение к вектору со структурами вершин каждого живого (isAlive()) объекта
	//Все структуры кладутся один общий вектор, получается массив всех живых объектов
	for(std::unordered_map<std::string, Object*>::iterator it = objectsMap.begin(); it != objectsMap.end(); it++) {
		if(it->second->isAlive()) {
			for(int i = 0; i < it->second->getVertices().size(); i++) {
				vertices.push_back((it->second->getVertices().at(i)));
			}
			totalV += it->second->getVertices().size();
		}
	}

	Vertex* v = new Vertex[totalV];

	for(int i = 0; i < vertices.size(); i++) {
		v[i] = vertices[i];
	}

	return v;
}

DWORD* ObjectManager::getIndices() {
	int totalInd = 0;
	std::vector<DWORD> indices;
	//Сходный с getVertices алгоритм
	for(std::unordered_map<std::string, Object*>::iterator it = objectsMap.begin(); it != objectsMap.end(); it++) {
		if(it->second->isAlive()) {
			for(int i = 0; i < it->second->getIndices().size(); i++) {
				indices.push_back((it->second->getIndices().at(i)));
			}
			totalInd += it->second->getIndices().size();
		}
	}

	DWORD* ind = new DWORD[totalInd];

	for(int i = 0; i < indices.size(); i++) {
		ind[i] = indices[i];
	}

	return ind;
}

int ObjectManager::totalVertices() {
	int totalV = 0;
	//Пробегаем в цикле for_each по контейнеру, считаем количество вершин у живых объектов
	std::for_each(objectsMap.begin(), objectsMap.end(), 
		[&](std::pair<std::string, Object*> obj) { if(obj.second->isAlive()) totalV += obj.second->getVertices().size(); });

	return totalV;
}

int ObjectManager::totalIndices() {
	int totalInd = 0;
	//Пробегаем в цикле for_each по контейнеру, считаем количество индексов у живых объектов
	std::for_each(objectsMap.begin(), objectsMap.end(), 
		[&](std::pair<std::string, Object*> obj) { if(obj.second->isAlive()) totalInd += obj.second->getIndices().size(); });

	return totalInd;
}

void ObjectManager::updateObjects() {
	std::for_each(objectsMap.begin(), objectsMap.end(), [&](std::pair<std::string, Object*> obj) 
																						{ obj.second->updatePosition();	});
}

void ObjectManager::getPositions(std::vector<std::pair<PxVec3, int>>& positions) {
	std::pair<PxVec3, int> tmp;
	std::for_each(objectsMap.begin(), objectsMap.end(), [&](std::pair<std::string, Object*> obj)
	{ if(obj.second->isAlive()) 
		{ tmp.first = obj.second->getPosition(), tmp.second = obj.second->getIndices().size(); positions.push_back(tmp); } 
	});
}

void ObjectManager::destroy() {
	std::for_each(objectsMap.begin(), objectsMap.end(), [&](std::pair<std::string, Object*> obj) {
		obj.second->destroy();
	});
}

void ObjectManager::getActors(std::vector<PxActor*>& actors) {
	std::for_each(objectsMap.begin(), objectsMap.end(), [&](std::pair<std::string, Object*> obj) {
		actors.push_back(obj.second->getActor());
	});
}

void ObjectManager::getObjects(std::vector<Object*>& objects) {
	std::for_each(objectsMap.begin(), objectsMap.end(), [&](std::pair<std::string, Object*> obj) {
		objects.push_back(obj.second);
	});
}