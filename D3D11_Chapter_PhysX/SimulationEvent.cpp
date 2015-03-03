#include "SimulationEvent.h"

void SimulationEvent::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
	for(PxU32 i=0; i < nbPairs; i++) {
		const PxContactPair& cp = pairs[i];

		//Здесь будет описано поведение объектов при коллизии
		//Пока что я просто хочу знать, что это-таки случилось
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