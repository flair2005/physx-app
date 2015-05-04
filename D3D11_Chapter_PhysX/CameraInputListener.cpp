#include "CameraInputListener.h"

CameraInputListener::CameraInputListener(Camera* pCamera) {
	m_pCameraSubject = pCamera;
}

//ƒвижение мыши
bool CameraInputListener::mouseMove(const MouseEvent& arg) {
//	m_pCameraSubject->rotate(arg.x, arg.y);
	return true;
}

// нопка нажата
bool CameraInputListener::keyPressed(const KeyEvent& arg) {
	switch(arg.code) {
	case eKeyCodes::KEY_A: 
		//m_pCameraSubject->moveLeft();						
		return true;
	case eKeyCodes::KEY_S:
		//m_pCameraSubject->moveBack();
		return true;
	case eKeyCodes::KEY_D:
		//m_pCameraSubject->moveRight();
		return true;
	case eKeyCodes::KEY_W:
		//m_pCameraSubject->moveForward();
		return true;
	default:
		return false;
	}
}