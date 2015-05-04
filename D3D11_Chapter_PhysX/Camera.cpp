#include "Camera.h"

Camera::Camera() {
	DefaultForward = XMVECTOR();
	DefaultRight = XMVECTOR();
	camForward = XMVECTOR();
	camRight = XMVECTOR();

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;
	camYaw = 0.0f;
	camPitch = 0.0f;
	rot = 0.01f;
	m_frameTime = 0.0f;
	lastX = 0.0f;
	lastY = 0.0f;

	camView = XMMatrixIdentity();

	camPosition = XMVECTOR();
	camTarget = XMVECTOR();
	camUp = XMVECTOR();
}

Camera::~Camera() {

}

void Camera::init(XMVECTOR position) {
	DefaultForward = XMVectorSet(0.0f,0.0f,1.0f, 0.0f);
	DefaultRight = XMVectorSet(1.0f,0.0f,0.0f, 0.0f);

	camForward = XMVectorSet(0.0f,0.0f,1.0f, 0.0f);
	camRight = XMVectorSet(1.0f,0.0f,0.0f, 0.0f);

	camPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	camPosition = position;
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

void Camera::update(double frameTime) {
	m_frameTime = frameTime;

	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix );
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateXTempMatrix, RotateYTempMatrix;
	RotateXTempMatrix = XMMatrixRotationX(camPitch);
	RotateYTempMatrix = XMMatrixRotationY(camYaw);
	XMMATRIX RotateTempMatrix = RotateXTempMatrix * RotateYTempMatrix;

	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateTempMatrix);

	camPosition += moveLeftRight * camRight;
	camPosition += moveBackForward * camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;	

	camView = XMMatrixLookAtLH( camPosition, camTarget, camUp );
}