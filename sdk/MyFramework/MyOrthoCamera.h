
#pragma once

#include "MyCamera.h"

class OrthoCamera : public Camera
{
public:
	OrthoCamera();
	virtual ~OrthoCamera();

	virtual void init(
		const MyVec3& eyeP,
		const MyVec3& targetP,
		float left,
		float right,
		float bottom,
		float top);

	// Setter

	void setLeft(float left);
	void setRight(float right);
	void setBottom(float bottom);
	void setTop(float top);

protected:
	virtual void computeProj(int width, int height);

protected:
	float m_left, m_right, m_bottom, m_top;
};