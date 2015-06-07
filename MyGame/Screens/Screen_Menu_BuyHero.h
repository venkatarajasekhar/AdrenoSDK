
#pragma once

#include "Screen_Menu.h"
#include <MyUIButton.h>
#include <MyUIList.h>

class BuyHeroMenuScreen : public MenuScreen, public IOnPressListener
{
private:
	// UI Widgets
	enum
	{
		BTN_BACK,
		NUM_BTNS,
	};

	enum
	{
		LIST_HERO_ON_SALE,
		NUM_LISTS,
	};

public:
	BuyHeroMenuScreen(ScreenManager* screenManager);
	~BuyHeroMenuScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

	void OnPress(const IOnPressListener::Data& data);

private:
	// UI Widgets
	UITextButton m_btns[NUM_BTNS];
	UIList m_list[NUM_LISTS];
};