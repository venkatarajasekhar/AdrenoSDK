
#pragma once

class ScreenManager;

class Screen
{
public:
	Screen(ScreenManager* screenManager);
	virtual ~Screen();

	virtual void init() = 0;
	virtual void resize(int width, int height) = 0;
	virtual void update(void* utilObjs) = 0;
	virtual void render(void* utilObjs) = 0;

protected:
	ScreenManager* m_screenManager;
};