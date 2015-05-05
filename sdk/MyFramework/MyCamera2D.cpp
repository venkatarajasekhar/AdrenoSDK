
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
	Rect2D viewport;
	viewport.Pos = MyVec2(0, 0);
	viewport.Size = MyVec2(width, height);

	changeViewport(viewport);

	Camera::resize(width, height);
}

void Camera2D::changeViewport(const Rect2D& viewport)
{
	int width, height;
	getWindowDimension(width, height);

	setLeft(viewport.Pos.x - (float)width / 2.0f);
	setRight(viewport.Pos.x + viewport.Size.x - (float)width / 2.0f);
	setTop(-viewport.Pos.y + (float)height / 2.0f);
	setBottom(-viewport.Pos.y - viewport.Size.y + (float)height / 2.0f);
}

void Camera2D::restoreDefaultViewport()
{
	int width, height;
	getWindowDimension(width, height);

	Rect2D viewport;
	viewport.Pos = MyVec2(0, 0);
	viewport.Size = MyVec2(width, height);

	changeViewport(viewport);
}