
#pragma once

#include "Screen_Menu.h"
#include <MyUIButton.h>

class GameOverScreen : public MenuScreen, public IOnPressListener
{
private:
	// UI Widgets
	enum
	{
		BTN_BACK_TO_MEMU,
		NUM_BTNS,
	};

	enum
	{
		LABEL_COMBAT_RESULT,
		NUM_LABELS,
	};

public:
	GameOverScreen(ScreenManager* screenManager);
	~GameOverScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

	void OnPress(const IOnPressListener::Data& data);
	void setTag(void* tag);

private:
	void changeUIPos();

private:
	// UI Widgets
	UITextButton m_btns[NUM_BTNS];
	UILabel m_labels[NUM_LABELS];
};