#pragma once

#include "DefHeaders.h"
#include "Log.h"

using namespace physx;

class SimulationEvent : public PxSimulationEventCallback {
private:

public:
	SimulationEvent() {}
	virtual ~SimulationEvent() {}

	//Переопределяем методы из PxSimulationEventCallback
	//Вызываются физиксом тогда, когда обнаруживается коллизия или срабатывает триггер
	//TODO: вообще, убрать бы их отсюда в отдельный класс, отвечающий чисто за события из PxSimulationEventCallback
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) {}
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
	virtual void onWake(PxActor** , PxU32 ) {}
	virtual void onSleep(PxActor** , PxU32 ) {}
};