
#pragma once

#include "MyOrthoCamera.h"

class Camera2D : public OrthoCamera
{
public:
	Camera2D();
	~Camera2D();

	void init();
	void resize(int width, int height);

	void changeViewport(const Rect2D& viewport);
	void restoreDefaultViewport();
};