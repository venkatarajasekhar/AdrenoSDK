
#pragma once

#include "MyCamera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera();
	virtual ~PerspectiveCamera();

	virtual void init(
		const MyVec3& eyeP,
		const MyVec3& targetP,
		float fovInDeg,
		float n,
		float f);

protected:
	virtual void computeProj(int width, int height);

protected:
	float m_fovInDeg;
	float m_near;
	float m_far;
};