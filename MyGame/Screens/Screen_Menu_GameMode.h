
#pragma once

#include "Screen_Menu.h"
#include <MyUIButton.h>

class GameModeMenuScreen : public MenuScreen, public IOnPressListener
{
private:
	// UI Widgets
	enum
	{
		IMAGE_BTN_PLAY_ONLINE,
		IMAGE_BTN_PLAY_SOLO,
		NUM_IMAGE_BTNS,
	};

public:
	GameModeMenuScreen(ScreenManager* screenManager);
	~GameModeMenuScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

	void OnPress(const IOnPressListener::Data& data);

private:
	// UI Widgets
	UIImageButton m_imageBtns[NUM_IMAGE_BTNS];
};