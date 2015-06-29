
#pragma once

#include "MyUtils.h"
#include "MyTexture.h"

class Screen;

class ScreenManager
{
public:
	ScreenManager();
	~ScreenManager();

	// Core
	void init(Texture& emptyTex);
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

	// Active screens
	void activeScreen(const MyString& id, void* tag = nullptr);
	void activePopupScreen(const MyString& id, void* tag = nullptr);
	void deactivePopupScreen();

	// Misc
	void addScreen(const MyString& id, Screen* screen, bool replaceIfExist = false);
	void removeScreen(const MyString& id);

private:
	Screen* getScreen(const MyString& id);

private:
	std::map<MyString, Screen*> m_screens;

	Screen* m_activeScreen;
	Screen* m_popupScreen;

	bool m_justActiveScreen;
	Texture* m_emptyTex;
};