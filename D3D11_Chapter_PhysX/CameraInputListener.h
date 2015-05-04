#pragma once

#include "InputListener.h"
#include "Camera.h"

class CameraInputListener : public InputListener {
private:
	Camera* m_pCameraSubject;
public:
	CameraInputListener(Camera* pCamera);
	virtual ~CameraInputListener() {}

		//�������� ����
	virtual bool mouseMove(const MouseEvent& arg);
		//������ ������
	virtual bool keyPressed(const KeyEvent& arg);
};