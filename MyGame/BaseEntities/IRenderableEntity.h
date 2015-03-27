
#pragma once

#include <MyInput.h>
#include <MyTimer.h>
#include <MyCamera.h>
#include <MyLight.h>

class IRenderableEntity
{
public:
	virtual void update(UserInput& userInput, Timer& timer) = 0;
	virtual void render(Camera& camera, Light& light) = 0;
};