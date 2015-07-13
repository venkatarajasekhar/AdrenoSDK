
// Always including Utils.h on top
#include "Utils.h"

#include <MyScreenManager.h>
#include "Screen_Menu_BuyHero.h"

#pragma region Constants

//=================================================================================================================================
//
// Constants
//
//=================================================================================================================================

static const MyVec2 DIAMOND_BAR_POS = MyVec2(50, 25);
static const float BTN_BUY_MARGIN = 10.0f;

#pragma endregion

#pragma region UIListItem_HeroOnSale class

//=================================================================================================================================
//
// UIListItem_HeroOnSale class
//
//=================================================================================================================================

class UIListItem_HeroOnSale : public UIListItem
{
public:
	UIListItem_HeroOnSale(
		UIList* list, 
		BuyHeroMenuScreen::HeroOnSale& heroOnSale,
		Texture& buyBtnBackground, 
		Font& font)
		: UIListItem(list)
	{
		m_heroOnSale = &heroOnSale;

		{
			MyColor textColor = MyColor(212.0f / 255.0f, 170.0f / 225.0f, 0.0f / 255.0f);
			m_buyBtn.init("", MyVec2(), buyBtnBackground, toString(heroOnSale.Price), font, textColor, 0.5f);
		}

		{
			MyVec2 size = MyVec2(
				m_heroOnSale->Preview->getWidth(),
				m_heroOnSale->Preview->getHeight() + BTN_BUY_MARGIN + buyBtnBackground.getHeight()
				);
			UIWidget::init("", MyVec2(), size);
		}
	}

	void update(UserInput& userInput)
	{
		m_buyBtn.update(userInput);

		UIWidget::update(userInput);
	}

	void render(SpriteBatch& spriteBatch, const Rect2D* viewport = nullptr)
	{
		Rect2D listViewport = m_list->getViewport();

		// Hero preview
		spriteBatch.renderTexture2D(
			m_heroOnSale->Preview,
			getPos(),
			0.0f,
			MyVec2(1.0f),
			&listViewport,
			(m_selected ? MyColor(0.8f, 0.8f, 0.8f) : MyColor(1, 1, 1)));

		// Buy button
		{
			MyVec2 pos = getPos() + MyVec2(
				0.5f * (m_heroOnSale->Preview->getWidth() - m_buyBtn.getSize().x),
				m_heroOnSale->Preview->getHeight() + BTN_BUY_MARGIN);
			m_buyBtn.setPos(pos);
			m_buyBtn.render(spriteBatch, &listViewport);
		}
	}

private:
	BuyHeroMenuScreen::HeroOnSale* m_heroOnSale;
	UITextButton m_buyBtn;
};

#pragma endregion

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

#pragma region initHeroOnSale

void BuyHeroMenuScreen::initHeroOnSale()
{
	m_heroOnSale[0].Preview = &m_textures[TEXTURE_BUY_HERO_FIGHTER_DANMEI];
	m_heroOnSale[0].Price = 950;

	m_heroOnSale[1].Preview = &m_textures[TEXTURE_BUY_HERO_FIGHTER_ELRATH];
	m_heroOnSale[1].Price = 2900;

	m_heroOnSale[2].Preview = &m_textures[TEXTURE_BUY_HERO_FIGHTER_TIAC];
	m_heroOnSale[2].Price = 1300;

	m_heroOnSale[3].Preview = &m_textures[TEXTURE_BUY_HERO_FIGHTER_VALLE];
	m_heroOnSale[3].Price = 270;

	m_heroOnSale[4].Preview = &m_textures[TEXTURE_BUY_HERO_GUARDIAN_DICK];
	m_heroOnSale[4].Price = 9000;

	m_heroOnSale[5].Preview = &m_textures[TEXTURE_BUY_HERO_GUARDIAN_SEWON];
	m_heroOnSale[5].Price = 8700;

	m_heroOnSale[6].Preview = &m_textures[TEXTURE_BUY_HERO_MAGE_CEZANNE];
	m_heroOnSale[6].Price = 4500;

	m_heroOnSale[7].Preview = &m_textures[TEXTURE_BUY_HERO_SHOOTER_RENOAH];
	m_heroOnSale[7].Price = 2300;
}

#pragma endregion

void BuyHeroMenuScreen::init()
{
	MenuScreen::init();
	initHeroOnSale();

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
	m_list[LIST_HERO_ON_SALE].setListPadding(30.0f);

	for (int i = 0; i < TOTAL_HEROES_ONSALE; i++)
	{
		m_list[LIST_HERO_ON_SALE].addItem(
			new UIListItem_HeroOnSale(
			&m_list[LIST_HERO_ON_SALE],
			m_heroOnSale[i],
			m_textures[TEXTURE_BUY_HERO_BTN_BUY],
			m_fonts[FONT_BAUHAUS93_26]));
	}

	// Other components
	{
		MyColor textColor = MyColor(212.0f / 255.0f, 170.0f / 225.0f, 0.0f / 255.0f);
		m_diamondBar.init(m_textures[TEXTURE_BUY_HERO_DIAMOND_BAR], m_fonts[FONT_BAUHAUS93_26], DIAMOND_BAR_POS, textColor, 0.7f);
	}
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
		float yOffset = 0.5f * (m_list[LIST_HERO_ON_SALE].getPos().y - m_btns[BTN_BACK].getSize().y);
		MyVec2 pos = m_list[LIST_HERO_ON_SALE].getPos() +
			MyVec2(
			0.5f * (m_list[LIST_HERO_ON_SALE].getSize().x - m_btns[BTN_BACK].getSize().x),
			m_list[LIST_HERO_ON_SALE].getSize().y + yOffset);

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

	// Other components
	m_diamondBar.update(*globalUtilObjs->userInput);
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

	// Other components
	m_diamondBar.render(*globalUtilObjs->spriteBatch);
}

void BuyHeroMenuScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "btn_back")
	{
		m_screenManager->activeScreen("MainMenuScreen");
	}
}