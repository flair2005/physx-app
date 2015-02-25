#include "PhysXError.h"

PhysXError::PhysXError() {
	
}

PhysXError::~PhysXError() {
	
}

void PhysXError::reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) {
	switch(code) {
	case PxErrorCode::eNO_ERROR:
		constructMessage("NO ERROR", message, file, line);
		break;
	case PxErrorCode::eDEBUG_INFO:
		constructMessage("DEBUG INFO", message, file, line);
		break;
	case PxErrorCode::eDEBUG_WARNING:
		constructMessage("DEBUG WARNING", message, file, line);
		break;
	case PxErrorCode::eINVALID_PARAMETER:
		constructMessage("INVALID PARAMETER", message, file, line);
		break;
	case PxErrorCode::eINVALID_OPERATION:
		constructMessage("INVALID OPERATION", message, file, line);
		break;
	case PxErrorCode::eINTERNAL_ERROR:
		constructMessage("INTERNAL ERROR", message, file, line);
		break;
	case PxErrorCode::eABORT:
		constructMessage("ABORT", message, file, line);
		break;
	case PxErrorCode::eMASK_ALL:
		constructMessage("MASK ALL", message, file, line);
		break;
	default:
		constructMessage("UNKNOWN PHYSX ERROR", message, file, line);
	}
}

void PhysXError::constructMessage(const char* str, const char* message, const char* file, int line) {
	Log::get()->err("%s: \"%s\" â %s::%d", str, message, file, line);
}