
#pragma once

#include "MyUtils.h"

class Screen;

class ScreenManager
{
public:
	ScreenManager();
	~ScreenManager();

	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

	void addScreen(const MyString& id, Screen* screen);
	void activeScreen(const MyString& id);

private:
	Screen* getScreen(const MyString& id);

private:
	Screen* m_activeScreen;
	std::map<MyString, Screen*> m_screens;
};