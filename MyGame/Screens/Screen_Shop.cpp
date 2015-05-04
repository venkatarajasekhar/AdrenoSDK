
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_Shop.h"
#include <MyScreenManager.h>

ShopScreen::ShopScreen(ScreenManager* screenManager)
	: Screen(screenManager),
	m_width(0),
	m_height(0)
{
}

ShopScreen::~ShopScreen()
{
}

void ShopScreen::init()
{
	// Assets textures
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/shop.pak").c_str());

		m_textures[TEXTURE_SHOP_BACKGROUND].init(resource.GetTexture("shop_background"));
		m_textures[TEXTURE_SHOP_BTN_CLOSE].init(resource.GetTexture("shop_btn_close"));
	}

	// Button widgets
	m_btns[BTN_CLOSE].init("shop_btn_close", MyVec2(), m_textures[TEXTURE_SHOP_BTN_CLOSE]);
	m_btns[BTN_CLOSE].addPressListener(this);
}

void ShopScreen::resize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void ShopScreen::update(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Button widgets
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].update(*globalUtilObjs->userInput);
	}
}

void ShopScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	{
		MyVec2 pos = 0.5f * MyVec2(
			m_width - m_textures[TEXTURE_SHOP_BACKGROUND].getWidth(),
			m_height - m_textures[TEXTURE_SHOP_BACKGROUND].getHeight());

		globalUtilObjs->spriteBatch->renderTexture2D(&m_textures[TEXTURE_SHOP_BACKGROUND], pos);

		pos.x += m_textures[TEXTURE_SHOP_BACKGROUND].getWidth() - m_btns[BTN_CLOSE].getSize().x;
		m_btns[BTN_CLOSE].setPos(pos);
	}

	// Button widgets
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].render(*globalUtilObjs->spriteBatch);
	}
}

void ShopScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "shop_btn_close")
	{
		m_screenManager->activeScreen("PlayScreen");
	}
}