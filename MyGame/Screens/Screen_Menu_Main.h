
#pragma once

#include "Screen_Menu.h"
#include <MyUIButton.h>

class MainMenuScreen : public MenuScreen, public IOnPressListener
{
private:
	// UI Widgets
	enum
	{
		BTN_PLAY_GAME,
		BTN_HEROES,
		BTN_ACHIEVEMENTS,
		BTN_LEADERBOARD,
		BTN_SETTINGS,
		BTN_PROFILE,
		NUM_BTNS,
	};

public:
	MainMenuScreen(ScreenManager* screenManager);
	~MainMenuScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

	void OnPress(const IOnPressListener::Data& data);

private:
	// UI Widgets
	UITextButton m_btns[NUM_BTNS];

	Rect2D m_btnContainerDest;
};