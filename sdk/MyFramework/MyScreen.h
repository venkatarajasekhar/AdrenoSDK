
#pragma once

#include "MyInput.h"
#include "MyTimer.h"
#include "MySpriteBatch.h"

class ScreenManager;

class Screen
{
public:
	Screen(ScreenManager* screenManager);
	virtual ~Screen();

	virtual void init() = 0;
	virtual void resize(int width, int height) = 0;
	virtual void update(UserInput& userInput, Timer& timer) = 0;
	virtual void render(SpriteBatch& spriteBatch) = 0;

protected:
	ScreenManager* m_screenManager;
};