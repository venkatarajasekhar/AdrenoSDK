
#include "MyCamera.h"

#pragma region Camera helpers

//========================================================================================================
//
// Camera helpers
//
//========================================================================================================

void Camera::computeView()
{
	m_view = createLookAtMatrix(m_eyeP, m_eyeP + m_lookV, m_upV);
}

void Camera::updateOrientation()
{
	m_lookV = normalize(m_lookV);

	m_rightV = cross(m_lookV, m_upV);
	m_rightV = normalize(m_rightV);

	m_upV = cross(m_rightV, m_lookV);
	m_upV = normalize(m_upV);
}

#pragma endregion

//========================================================================================================
//
// Camera class
//
//========================================================================================================

Camera::Camera()
	: m_eyeP(0.0f, 0.0f, 1.0f),
	m_lookV(0.0f, 0.0f, -1.0f),
	m_upV(0.0f, 1.0f, 0.0f),
	m_rightV(1.0f, 0.0f, 0.0f)
{
}

Camera::~Camera()
{
}

void Camera::init(const MyVec3& eyeP, const MyVec3& targetP)
{
	m_eyeP = eyeP;
	m_lookV = targetP - eyeP;

	updateOrientation();
}

void Camera::resize(int width, int height)
{
	computeProj(width, height);
}

void Camera::update(Timer& timer, UserInput& userInput)
{
	updateOrientation();
	computeView();
}

// Getter

const MyVec3& Camera::getEye()const
{
	return m_eyeP;
}

const MyMat4& Camera::getView()const
{
	return m_view;
}

const MyMat4& Camera::getProj()const
{
	return m_proj;
}

// Setter

void Camera::setEye(const MyVec3& eyeP)
{
	m_eyeP = eyeP;
}

void Camera::setLook(const MyVec3& lookV)
{
	m_lookV = lookV;
}