#pragma once

#include "DefHeaders.h"
#include "Log.h"

using namespace physx;

//Чтобы не использовать PxDefaultErrorCallback, определяем своего наследника от абстрактного physx::PxErrorCallback
class PhysXError : public PxErrorCallback {
private:
	void constructMessage(const char* str, const char* message, const char* file, int line);
public:
	PhysXError();
	~PhysXError();

	void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line);
};