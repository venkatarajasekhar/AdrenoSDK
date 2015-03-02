
#pragma once

#include "MyInput.h"
#include "MyTimer.h"
#include "MySpriteBatch.h"

class Screen;

class ScreenManager
{
public:
	ScreenManager();
	~ScreenManager();

	void resize(int width, int height);
	void update(UserInput& userInput, Timer& timer);
	void render(SpriteBatch& spriteBatch);

	void addScreen(const MyString& id, Screen* screen);
	void activeScreen(const MyString& id);

private:
	Screen* getScreen(const MyString& id);

private:
	Screen* m_activeScreen;
	std::map<MyString, Screen*> m_screens;

	int m_width, m_height;
};