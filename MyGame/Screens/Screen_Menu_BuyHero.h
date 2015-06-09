
#pragma once

#include "Screen_Menu.h"
#include "InfoBar.h"
#include <MyUIButton.h>
#include <MyUIList.h>

class BuyHeroMenuScreen : public MenuScreen, public IOnPressListener
{
public:
	struct HeroOnSale
	{
		Texture* Preview;
		int Price;
	};

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
	void initHeroOnSale();

private:
	static const int TOTAL_HEROES_ONSALE = 8;

private:
	// UI Widgets
	UITextButton m_btns[NUM_BTNS];
	UIList m_list[NUM_LISTS];

	// Other components
	InfoBar m_diamondBar;

	HeroOnSale m_heroOnSale[TOTAL_HEROES_ONSALE];
};