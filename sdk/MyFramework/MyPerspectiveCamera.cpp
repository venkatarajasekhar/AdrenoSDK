
#include "MyPerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera()
	: m_fovInDeg(45.0f),
	m_near(0.1f),
	m_far(100.0f)
{
}

PerspectiveCamera::~PerspectiveCamera()
{
}

void PerspectiveCamera::init(
	const MyVec3& eyeP,
	const MyVec3& targetP,
	float fovInDeg,
	float n,
	float f)
{
	m_fovInDeg = fovInDeg;
	m_near = n;
	m_far = f;

	Camera::init(eyeP, targetP);
}

void PerspectiveCamera::computeProj(int width, int height)
{
	float aspect = (float)width / (float)height;
	m_proj = createPerspectiveMatrix(m_fovInDeg, aspect, m_near, m_far);
}