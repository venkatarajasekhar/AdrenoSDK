
#include "MyOrthoCamera.h"

OrthoCamera::OrthoCamera()
	: m_left(-1.0f),
	m_right(+1.0f),
	m_bottom(-1.0f),
	m_top(+1.0f)
{
}

OrthoCamera::~OrthoCamera()
{
}

void OrthoCamera::init(
	const MyVec3& eyeP,
	const MyVec3& targetP,
	float left,
	float right,
	float bottom,
	float top)
{
	m_left = left;
	m_right = right;
	m_bottom = bottom;
	m_top = top;

	Camera::init(eyeP, targetP);
}

void OrthoCamera::computeProj(int width, int height)
{
	m_proj = createOrthoMatrix(m_left, m_right, m_bottom, m_top);
}

// Setter

void OrthoCamera::setLeft(float left)
{
	m_left = left;
	computeProj(0, 0);
}

void OrthoCamera::setRight(float right)
{
	m_right = right;
	computeProj(0, 0);
}

void OrthoCamera::setBottom(float bottom)
{
	m_bottom = bottom;
	computeProj(0, 0);
}

void OrthoCamera::setTop(float top)
{
	m_top = top;
	computeProj(0, 0);
}