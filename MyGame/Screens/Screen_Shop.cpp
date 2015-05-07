
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_Shop.h"
#include <MyScreenManager.h>

//==============================================================================================================
//
// Constant
//
//==============================================================================================================

static const MyVec2 LIST_ITEM_MARGIN = MyVec2(15, 30);

//==============================================================================================================
//
// Some class declarations
//
//==============================================================================================================

class UIListItem_ShopItem : public UIListItem
{
public:
	UIListItem_ShopItem(
		UIList* list, 
		Texture& background,
		Texture& itemAvatar,
		Texture& goldIcon,
		Texture& btnBuyBackground,
		const MyString& name,
		int price,
		Font& font)
		: UIListItem(list)
	{
		m_background = &background;
		m_itemAvatar = &itemAvatar;
		m_goldIcon = &goldIcon;

		m_buyBtn.init("", MyVec2(), btnBuyBackground, "Buy", font);
		m_name.init("", MyVec2(), font, name);
		m_price.init("", MyVec2(), font, toString(price));

		UIWidget::init("", MyVec2(), MyVec2(m_background->getWidth(), m_background->getHeight()));
	}

	void render(SpriteBatch& spriteBatch, const Rect2D* viewport = nullptr)
	{
		spriteBatch.renderTexture2D(m_background, m_bounding, nullptr, 0.0f, &m_list->getViewport());

		{
			float offset = 0.5f * (getSize().y - m_itemAvatar->getHeight());
			MyVec2 pos = getPos() + MyVec2(offset);
			spriteBatch.renderTexture2D(m_itemAvatar, pos, 0.0f, MyVec2(1), &m_list->getViewport());

			{
				pos.x += m_itemAvatar->getWidth() + offset;
				m_name.setPos(pos);
				m_name.render(spriteBatch, &m_list->getViewport());
			}

			{
				pos.y += 30;
				m_price.setPos(pos);
				m_price.render(spriteBatch, &m_list->getViewport());
			}

			{
				pos.x += m_price.getSize().x + 10;
				spriteBatch.renderTexture2D(m_goldIcon, pos, 0.0f, MyVec2(1), &m_list->getViewport());
			}
		}

		{
			float offset = 0.5f * (getSize().y - m_buyBtn.getSize().y);
			MyVec2 pos = getPos() + MyVec2(getSize().x - m_buyBtn.getSize().x - 5.0f, offset);
			m_buyBtn.setPos(pos);
			m_buyBtn.render(spriteBatch, &m_list->getViewport());
		}
	}

private:
	Texture* m_background;
	Texture* m_itemAvatar;
	Texture* m_goldIcon;
	UITextButton m_buyBtn;
	UILabel m_name;
	UILabel m_price;
};

//==============================================================================================================
//
// ShopScreen class
//
//==============================================================================================================

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
		m_textures[TEXTURE_LIST_ITEM_BACKGROUND].init(resource.GetTexture("list_item_background"));
		m_textures[TEXTURE_ITEM_BACKGROUND].init(resource.GetTexture("item_background"));
		m_textures[TEXTURE_GOLD_ICON].init(resource.GetTexture("gold_icon"));
		m_textures[TEXTURE_SHOP_BTN_BACKGROUND].init(resource.GetTexture("shop_btn_background"));
		m_textures[TEXTURE_ITEM_BLOOD_POUCH].init(resource.GetTexture("item_blood_pouch"));
		m_textures[TEXTURE_ITEM_CHAIN_MAIL].init(resource.GetTexture("item_chain_mail"));
		m_textures[TEXTURE_ITEM_CLOAK_OF_THE_RESISTANT].init(resource.GetTexture("item_cloak_of_the_resistant"));
		m_textures[TEXTURE_ITEM_LIFE_WARD].init(resource.GetTexture("item_life_ward"));
		m_textures[TEXTURE_ITEM_LIGHT_CALVARY_HAT].init(resource.GetTexture("item_light_calvary_hat"));
		m_textures[TEXTURE_ITEM_STAFF_OF_SATHLENAR].init(resource.GetTexture("item_staff_of_sathlenar"));
	}

	// Fonts assets
	m_fonts[FONT_CONSOLAS_12].init(resolveAssetsPath("Fonts/Consolas12.pak"));

	// Button widgets
	m_btns[BTN_CLOSE].init("shop_btn_close", MyVec2(), m_textures[TEXTURE_SHOP_BTN_CLOSE]);
	m_btns[BTN_CLOSE].addPressListener(this);

	// List widgets
	m_shopItemList.init("shop_item_list", MyVec2(0, 0), m_textures[TEXTURE_LIST_ITEM_BACKGROUND]);
	m_shopItemList.addItem(new UIListItem_ShopItem(
		&m_shopItemList, 
		m_textures[TEXTURE_ITEM_BACKGROUND], 
		m_textures[TEXTURE_ITEM_BLOOD_POUCH], 
		m_textures[TEXTURE_GOLD_ICON], 
		m_textures[TEXTURE_SHOP_BTN_BACKGROUND], 
		"Blood Pouch", 
		200, 
		m_fonts[FONT_CONSOLAS_12]));
	m_shopItemList.addItem(new UIListItem_ShopItem(
		&m_shopItemList,
		m_textures[TEXTURE_ITEM_BACKGROUND],
		m_textures[TEXTURE_ITEM_CHAIN_MAIL],
		m_textures[TEXTURE_GOLD_ICON],
		m_textures[TEXTURE_SHOP_BTN_BACKGROUND],
		"Chain Mail",
		1600,
		m_fonts[FONT_CONSOLAS_12]));
	m_shopItemList.addItem(new UIListItem_ShopItem(
		&m_shopItemList,
		m_textures[TEXTURE_ITEM_BACKGROUND],
		m_textures[TEXTURE_ITEM_CLOAK_OF_THE_RESISTANT],
		m_textures[TEXTURE_GOLD_ICON],
		m_textures[TEXTURE_SHOP_BTN_BACKGROUND],
		"Cloak of the...",
		3000,
		m_fonts[FONT_CONSOLAS_12]));
	m_shopItemList.addItem(new UIListItem_ShopItem(
		&m_shopItemList,
		m_textures[TEXTURE_ITEM_BACKGROUND],
		m_textures[TEXTURE_ITEM_LIFE_WARD],
		m_textures[TEXTURE_GOLD_ICON],
		m_textures[TEXTURE_SHOP_BTN_BACKGROUND],
		"Life Ward",
		550,
		m_fonts[FONT_CONSOLAS_12]));
	m_shopItemList.addItem(new UIListItem_ShopItem(
		&m_shopItemList,
		m_textures[TEXTURE_ITEM_BACKGROUND],
		m_textures[TEXTURE_ITEM_LIGHT_CALVARY_HAT],
		m_textures[TEXTURE_GOLD_ICON],
		m_textures[TEXTURE_SHOP_BTN_BACKGROUND],
		"Light Cal...",
		5000,
		m_fonts[FONT_CONSOLAS_12]));
	m_shopItemList.addItem(new UIListItem_ShopItem(
		&m_shopItemList,
		m_textures[TEXTURE_ITEM_BACKGROUND],
		m_textures[TEXTURE_ITEM_STAFF_OF_SATHLENAR],
		m_textures[TEXTURE_GOLD_ICON],
		m_textures[TEXTURE_SHOP_BTN_BACKGROUND],
		"Staff of Sa...",
		790,
		m_fonts[FONT_CONSOLAS_12]));
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

	// List widgets
	m_shopItemList.update(*globalUtilObjs->userInput);
}

void ShopScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	{
		MyVec2 pos = 0.5f * MyVec2(
			m_width - m_textures[TEXTURE_SHOP_BACKGROUND].getWidth(),
			m_height - m_textures[TEXTURE_SHOP_BACKGROUND].getHeight());

		globalUtilObjs->spriteBatch->renderTexture2D(&m_textures[TEXTURE_SHOP_BACKGROUND], pos);

		m_shopItemList.setPos(pos + LIST_ITEM_MARGIN);

		pos.x += m_textures[TEXTURE_SHOP_BACKGROUND].getWidth() - m_btns[BTN_CLOSE].getSize().x;
		m_btns[BTN_CLOSE].setPos(pos);
	}

	// Button widgets
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].render(*globalUtilObjs->spriteBatch);
	}

	// List widgets
	m_shopItemList.render(*globalUtilObjs->spriteBatch);
}

void ShopScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "shop_btn_close")
	{
		m_screenManager->activeScreen("PlayScreen");
	}
}