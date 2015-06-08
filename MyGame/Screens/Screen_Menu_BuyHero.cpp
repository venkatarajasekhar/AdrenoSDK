
// Always including Utils.h on top
#include "Utils.h"

#include <MyScreenManager.h>
#include "Screen_Menu_BuyHero.h"

//=================================================================================================================================
//
// Constants
//
//=================================================================================================================================

static const float BTN_BACK_MARGIN = 10.0f;

//=================================================================================================================================
//
// BuyHeroMenuScreen class
//
//=================================================================================================================================

BuyHeroMenuScreen::BuyHeroMenuScreen(ScreenManager* screenManager)
	: MenuScreen(screenManager)
{
}

BuyHeroMenuScreen::~BuyHeroMenuScreen()
{
}

void BuyHeroMenuScreen::init()
{
	MenuScreen::init();

	// Buttons
	{
		MyColor textColor = MyColor(212.0f / 255.0f, 170.0f / 225.0f, 0.0f / 255.0f);

		m_btns[BTN_BACK].init(
			"btn_back",
			MyVec2(),
			m_textures[TEXTURE_COMMON_BTN_BACKGROUND],
			"BACK",
			m_fonts[FONT_BAUHAUS93_26],
			textColor);
	}

	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].addPressListener(this);
	}

	// List
	m_list[LIST_HERO_ON_SALE].init("list_hero_on_sale", MyVec2(), m_textures[TEXTURE_COMMON_HPANEL], UIList::HORIZONTAL);
}

void BuyHeroMenuScreen::resize(int width, int height)
{
	MenuScreen::resize(width, height);

	// List
	{
		MyVec2 pos = MyVec2(
			0.5f * (m_width - m_list[LIST_HERO_ON_SALE].getSize().x),
			0.5f * (m_height - m_list[LIST_HERO_ON_SALE].getSize().y));

		m_list[LIST_HERO_ON_SALE].setPos(pos);
	}

	// Button
	{
		MyVec2 pos = m_list[LIST_HERO_ON_SALE].getPos() +
			MyVec2(
			0.5f * (m_list[LIST_HERO_ON_SALE].getSize().x - m_btns[BTN_BACK].getSize().x),
			m_list[LIST_HERO_ON_SALE].getSize().y + BTN_BACK_MARGIN);

		m_btns[BTN_BACK].setPos(pos);
	}
}

void BuyHeroMenuScreen::update(void* utilObjs)
{
	MenuScreen::update(utilObjs);

	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Buttons
	for (size_t i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].update(*globalUtilObjs->userInput);
	}

	// List
	for (size_t i = 0; i < NUM_LISTS; i++)
	{
		m_list[i].update(*globalUtilObjs->userInput);
	}
}

void BuyHeroMenuScreen::render(void* utilObjs)
{
	MenuScreen::render(utilObjs);

	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Buttons
	for (size_t i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].render(*globalUtilObjs->spriteBatch);
	}

	// List
	for (size_t i = 0; i < NUM_LISTS; i++)
	{
		m_list[i].render(*globalUtilObjs->spriteBatch);
	}
}

void BuyHeroMenuScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "btn_back")
	{
		m_screenManager->activeScreen("MainMenuScreen");
	}
}