#pragma once

#include "DefHeaders.h"

#define CAMERA_SPEED_MULTIPLICATOR	50.0f
#define CAMERA_ROTATE_MULTIPLICATOR	0.001f

class Camera {
private:
	float moveLeftRight;
	float moveBackForward;
	float camYaw;
	float camPitch;
	float rot;

	float m_frameTime;

	float lastX;
	float lastY;

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;

	XMVECTOR DefaultForward;
	XMVECTOR DefaultRight;
	XMVECTOR camForward;
	XMVECTOR camRight;
	XMMATRIX camRotationMatrix;

	XMMATRIX camView;
public:
	Camera();
	~Camera();

	XMMATRIX getView()					{ return camView;	}

	void init(XMVECTOR position);
	void update(double frameTime);

	void* operator new(size_t i)		{ return _aligned_malloc(i, 16);	}
	void operator delete(void* p)		{ _aligned_free(p);					}

	void moveLeft(double dt)			{ moveLeftRight	-= CAMERA_SPEED_MULTIPLICATOR * dt; }
	void moveRight(double dt)			{ moveLeftRight	+= CAMERA_SPEED_MULTIPLICATOR * dt; }
	void moveForward(double dt)			{ moveBackForward += CAMERA_SPEED_MULTIPLICATOR * dt; }
	void moveBack(double dt)			{ moveBackForward -= CAMERA_SPEED_MULTIPLICATOR * dt; }

	void rotate(float newYaw, float newPitch)	{camYaw += newYaw; camPitch += newPitch; }
};