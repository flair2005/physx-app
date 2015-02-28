#pragma once

#include "DefHeaders.h"
#include "Log.h"

using namespace physx;

//����� �� ������������ PxDefaultErrorCallback, ���������� ������ ���������� �� ������������ physx::PxErrorCallback
class PhysXError : public PxErrorCallback {
private:
	void constructMessage(const char* str, const char* message, const char* file, int line);
public:
	PhysXError();
	~PhysXError();

	void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line);
};