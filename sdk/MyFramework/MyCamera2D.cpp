
#include "MyCamera2D.h"

Camera2D::Camera2D()
{
}

Camera2D::~Camera2D()
{
}

void Camera2D::init()
{
	Camera::init(MyVec3(0.0f, 0.0f, 1.0f), MyVec3(0.0f, 0.0f, 0.0f));
}

void Camera2D::resize(int width, int height)
{
	m_left = -(float)width / 2.0f;
	m_right = +(float)width / 2.0f;
	m_bottom = -(float)height / 2.0f;
	m_top = +(float)height / 2.0f;

	Camera::resize(width, height);
}