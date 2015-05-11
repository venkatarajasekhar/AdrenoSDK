
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_Shop.h"
#include <MyScreenManager.h>

#pragma region Constants

//==============================================================================================================
//
// Constants
//
//==============================================================================================================

static const float LIST_ITEM_VMARGIN = 30.0f;

#pragma endregion

#pragma region UIListItem_ShopItem class

//==============================================================================================================
//
// UIListItem_ShopItem class
//
//==============================================================================================================

class UIListItem_ShopItem : public UIListItem
{
public:
	UIListItem_ShopItem(
		UIList* list, 
		Texture& background,
		Texture& goldIcon,
		Texture& btnBuyBackground,
		Font& font,
		ItemInfo& itemInfo)
		: UIListItem(list)
	{
		m_itemInfo = itemInfo;
		m_background = &background;
		m_itemAvatar = itemInfo.Avatar;
		m_goldIcon = &goldIcon;

		m_buyBtn.init("", MyVec2(), btnBuyBackground, "Buy", font);
		m_name.init("", MyVec2(), font, itemInfo.Name, 150.0f, UILabel::CUT_DOWN_WITH_ELLIPSIS);
		m_price.init("", MyVec2(), font, toString(itemInfo.Price));

		UIWidget::init("", MyVec2(), MyVec2(m_background->getWidth(), m_background->getHeight()));
	}

	void render(SpriteBatch& spriteBatch, const Rect2D* viewport = nullptr)
	{
		Rect2D listViewport = m_list->getViewport();

		spriteBatch.renderTexture2D(m_background, m_bounding, nullptr, 0.0f, &listViewport, (m_selected ? MyColor(0.7f, 0.7f, 0.7f) : MyColor(1, 1, 1)));

		{
			float offset = 0.5f * (getSize().y - m_itemAvatar->getHeight());
			MyVec2 pos = getPos() + MyVec2(offset);
			spriteBatch.renderTexture2D(m_itemAvatar, pos, 0.0f, MyVec2(1), &listViewport);

			{
				pos.x += m_itemAvatar->getWidth() + offset;
				m_name.setPos(pos);
				m_name.render(spriteBatch, &listViewport);
			}

			{
				pos.y += 30;
				m_price.setPos(pos);
				m_price.render(spriteBatch, &listViewport);
			}

			{
				pos.x += m_price.getSize().x + 10;
				spriteBatch.renderTexture2D(m_goldIcon, pos, 0.0f, MyVec2(1), &listViewport);
			}
		}

		{
			float offset = 0.5f * (getSize().y - m_buyBtn.getSize().y);
			MyVec2 pos = getPos() + MyVec2(getSize().x - m_buyBtn.getSize().x - 5.0f, offset);
			m_buyBtn.setPos(pos);
			m_buyBtn.render(spriteBatch, &listViewport);
		}
	}

	ItemInfo getItemInfo()
	{
		return m_itemInfo;
	}

private:
	Texture* m_background;
	Texture* m_itemAvatar;
	Texture* m_goldIcon;
	UITextButton m_buyBtn;
	UILabel m_name;
	UILabel m_price;

	ItemInfo m_itemInfo;
};

#pragma endregion

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

#pragma region Init items info

void ShopScreen::initItemInfo()
{
	m_itemInfo[0].Name = "Chain Mail";
	m_itemInfo[0].Desc = "Painstakingly handmade by craftsmen in the town of Silence.";
	m_itemInfo[0].Price = 250;
	m_itemInfo[0].Benefit = "+ 10 Physical Defense";
	m_itemInfo[0].Avatar = (m_textures + TEXTURE_ITEM_CHAIN_MAIL);

	m_itemInfo[1].Name = "Cloak of the Resistant";
	m_itemInfo[1].Desc = "A cloak woven of hair from the mane of the Night Horse, which legend says roams the Whispering Islands every blue moon.";
	m_itemInfo[1].Price = 250;
	m_itemInfo[1].Benefit = "+ 10 Magical Defense";
	m_itemInfo[1].Avatar = (m_textures + TEXTURE_ITEM_CLOAK_OF_THE_RESISTANT);

	m_itemInfo[2].Name = "Life Ward";
	m_itemInfo[2].Desc = "A ring blessed by an Orc mage to bring health to any lucky enough to wear it.";
	m_itemInfo[2].Price = 450;
	m_itemInfo[2].Benefit = "+ 3 HP Regeneration";
	m_itemInfo[2].Avatar = (m_textures + TEXTURE_ITEM_LIFE_WARD);

	m_itemInfo[3].Name = "Blood Pouch";
	m_itemInfo[3].Desc = "A trick from the playbook of traveling magicians, this pouch deceives enemies into thinking the holder is gravely injured.";
	m_itemInfo[3].Price = 450;
	m_itemInfo[3].Benefit = "+ 150 HP";
	m_itemInfo[3].Avatar = (m_textures + TEXTURE_ITEM_BLOOD_POUCH);

	m_itemInfo[4].Name = "Staff of Sathlenar";
	m_itemInfo[4].Desc = "A staff stolen from the mighty Sathlenar long ago, It still retains remnants of his power.";
	m_itemInfo[4].Price = 675;
	m_itemInfo[4].Benefit = "+ 270 MP";
	m_itemInfo[4].Avatar = (m_textures + TEXTURE_ITEM_STAFF_OF_SATHLENAR);

	m_itemInfo[5].Name = "Light Calvary Hat";
	m_itemInfo[5].Desc = "A cavalry hat woven from threads of tempered steel, perfected by a skilled seamstress from the East.";
	m_itemInfo[5].Price = 750;
	m_itemInfo[5].Benefit = "+ 250 HP";
	m_itemInfo[5].Avatar = (m_textures + TEXTURE_ITEM_LIGHT_CALVARY_HAT);
}

#pragma endregion

void ShopScreen::init()
{
	// Assets textures
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/shop.pak").c_str());

		// Background panel
		m_textures[TEXTURE_SHOP_BACKGROUND].init(resource.GetTexture("shop_background"));
		m_textures[TEXTURE_LIST_ITEM_BACKGROUND].init(resource.GetTexture("list_item_background"));
		m_textures[TEXTURE_DESC_ITEM_BACKGROUND].init(resource.GetTexture("desc_item_background"));
		m_textures[TEXTURE_ITEM_BACKGROUND].init(resource.GetTexture("item_background"));

		// Button
		m_textures[TEXTURE_SHOP_BTN_CLOSE].init(resource.GetTexture("shop_btn_close"));
		m_textures[TEXTURE_SHOP_BTN_BACKGROUND].init(resource.GetTexture("shop_btn_background"));
		
		// Icon
		m_textures[TEXTURE_GOLD_ICON].init(resource.GetTexture("gold_icon"));
		
		// Item avatar
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

	// Label widgets
	m_labels[LABEL_ITEM_DESC].init("", MyVec2(), m_fonts[FONT_CONSOLAS_12], "Please choose an item to show", m_textures[TEXTURE_DESC_ITEM_BACKGROUND].getWidth());

	// List widgets
	initItemInfo();

	m_shopItemList.init("shop_item_list", MyVec2(0, 0), m_textures[TEXTURE_LIST_ITEM_BACKGROUND]);
	m_shopItemList.addPressListItemListener(this);

	for (int i = 0; i < NUM_ITEMS; i++)
	{
		m_shopItemList.addItem(new UIListItem_ShopItem(
			&m_shopItemList,
			m_textures[TEXTURE_ITEM_BACKGROUND],
			m_textures[TEXTURE_GOLD_ICON],
			m_textures[TEXTURE_SHOP_BTN_BACKGROUND],
			m_fonts[FONT_CONSOLAS_12],
			m_itemInfo[i]));
	}
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

	// Label widgets
	for (int i = 0; i < NUM_LABELS; i++)
	{
		m_labels[i].update(*globalUtilObjs->userInput);
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

		{
			float xOffset = (
				m_textures[TEXTURE_SHOP_BACKGROUND].getWidth() - 
				m_shopItemList.getSize().x - 
				m_textures[TEXTURE_DESC_ITEM_BACKGROUND].getWidth()) / 3.0f;

			m_shopItemList.setPos(pos + MyVec2(xOffset, LIST_ITEM_VMARGIN));

			xOffset = 2 * xOffset + m_shopItemList.getSize().x;
			float yOffset = LIST_ITEM_VMARGIN + m_shopItemList.getSize().y - m_textures[TEXTURE_DESC_ITEM_BACKGROUND].getHeight();

			globalUtilObjs->spriteBatch->renderTexture2D(&m_textures[TEXTURE_DESC_ITEM_BACKGROUND], pos + MyVec2(xOffset, yOffset));
			m_labels[LABEL_ITEM_DESC].setPos(pos + MyVec2(xOffset, yOffset) + MyVec2(5.0f));
			m_labels[LABEL_ITEM_DESC].render(*globalUtilObjs->spriteBatch);
		}
		
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

void ShopScreen::OnPressListItem(const IOnPressListItemListener::Data& data)
{
	if (data.Id == "shop_item_list")
	{
		UIListItem_ShopItem* item = (UIListItem_ShopItem*)data.ListItem;

		MyString desc = item->getItemInfo().Name;
		desc += "\n\n";
		desc += "Cost: " + toString(item->getItemInfo().Price);
		desc += "\n";
		desc += "Benefit: " + item->getItemInfo().Benefit;
		desc += "\n\n";
		desc += item->getItemInfo().Desc;

		m_labels[LABEL_ITEM_DESC].setText(desc);
	}
}