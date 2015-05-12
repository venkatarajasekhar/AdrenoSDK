
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_Shop.h"
#include <MyScreenManager.h>

#pragma region Event class

//===========================================================================================================
//
// Event class
//
//===========================================================================================================

void OnBuyItemListenee::addBuyItemListener(IOnBuyItemListener* listener)
{
	if (listener == nullptr) return;

	bool existed(false);
	for (auto i = m_buyItemListeners.begin(); i != m_buyItemListeners.end(); ++i)
	{
		if ((*i) == listener)
		{
			existed = true;
			break;
		}
	}
	if (!existed)
	{
		m_buyItemListeners.push_back(listener);
	}
}

void OnBuyItemListenee::throwBuyItemEvent(IOnBuyItemListener::Data& data)
{
	for (auto i = m_buyItemListeners.begin(); i != m_buyItemListeners.end(); ++i)
	{
		if ((*i) != nullptr)
		{
			(*i)->OnBuyItemItem(data);
		}
	}
}

#pragma endregion

#pragma region UIListItem_ShopItem class

//==============================================================================================================
//
// UIListItem_ShopItem class
//
//==============================================================================================================

class UIListItem_ShopItem : public UIListItem, public IOnPressListener, public OnBuyItemListenee
{
public:
	UIListItem_ShopItem(
		UIList* list, 
		IOnBuyItemListener* buyItemListener,
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

		addBuyItemListener(buyItemListener);

		m_buyBtn.init("buy_item_btn", MyVec2(), btnBuyBackground, "Buy", font);
		m_buyBtn.addPressListener(this);
		m_name.init("", MyVec2(), font, itemInfo.Name, 150.0f, UILabel::CUT_DOWN_WITH_ELLIPSIS);
		m_price.init("", MyVec2(), font, toString(itemInfo.Price));

		UIWidget::init("", MyVec2(), MyVec2(m_background->getWidth(), m_background->getHeight()));
	}

	void update(UserInput& userInput)
	{
		m_buyBtn.update(userInput);

		UIWidget::update(userInput);
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

	void OnPress(const IOnPressListener::Data& data)
	{
		if (data.Id == "buy_item_btn")
		{
			IOnBuyItemListener::Data data("", &m_itemInfo);
			throwBuyItemEvent(data);
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

#pragma region UIListItem_SelectedItem class

//==============================================================================================================
//
// UIListItem_SelectedItem class
//
//==============================================================================================================

class UIListItem_SelectedItem : public UIListItem
{
public:
	UIListItem_SelectedItem(UIList* list, Texture& texture)
		: UIListItem(list)
	{
		m_texture = &texture;

		UIWidget::init("", MyVec2(), MyVec2(m_texture->getWidth(), m_texture->getHeight()));
	}

	void render(SpriteBatch& spriteBatch, const Rect2D* viewport = nullptr)
	{
		Rect2D listViewport = m_list->getViewport();

		spriteBatch.renderTexture2D(
			m_texture, 
			m_bounding, 
			nullptr, 
			0.0f, 
			&listViewport, 
			(m_selected ? MyColor(0, 1, 1) : MyColor(1, 1, 1)));
	}

private:
	Texture* m_texture;
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
		m_textures[TEXTURE_LIST_SELECTED_ITEM_BACKGROUND].init(resource.GetTexture("list_selected_item_background"));
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

	m_list[LIST_ITEM].init("shop_item_list", MyVec2(0, 0), m_textures[TEXTURE_LIST_ITEM_BACKGROUND]);
	m_list[LIST_ITEM].addPressListItemListener(this);

	for (int i = 0; i < NUM_ITEMS; i++)
	{
		m_list[LIST_ITEM].addItem(new UIListItem_ShopItem(
			&m_list[LIST_ITEM],
			this,
			m_textures[TEXTURE_ITEM_BACKGROUND],
			m_textures[TEXTURE_GOLD_ICON],
			m_textures[TEXTURE_SHOP_BTN_BACKGROUND],
			m_fonts[FONT_CONSOLAS_12],
			m_itemInfo[i]));
	}

	m_list[LIST_SELECTED_ITEM].init("selected_item_list", MyVec2(), m_textures[TEXTURE_LIST_SELECTED_ITEM_BACKGROUND], UIList::HORIZONTAL);
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
	for (int i = 0; i < NUM_LISTS; i++)
	{
		m_list[i].update(*globalUtilObjs->userInput);
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

		{
			float xOffset = (
				m_textures[TEXTURE_SHOP_BACKGROUND].getWidth() - 
				m_list[LIST_ITEM].getSize().x -
				m_textures[TEXTURE_DESC_ITEM_BACKGROUND].getWidth()) / 3.0f;
			float yOffset = (
				m_textures[TEXTURE_SHOP_BACKGROUND].getHeight() -
				m_list[LIST_ITEM].getSize().y -
				m_list[LIST_SELECTED_ITEM].getSize().y) / 3.0f;

			m_list[LIST_ITEM].setPos(pos + MyVec2(xOffset, yOffset));
			m_list[LIST_SELECTED_ITEM].setPos(pos + MyVec2(xOffset, 2 * yOffset + m_list[LIST_ITEM].getSize().y));

			xOffset = 2 * xOffset + m_list[LIST_ITEM].getSize().x;
			yOffset = yOffset + m_list[LIST_ITEM].getSize().y - m_textures[TEXTURE_DESC_ITEM_BACKGROUND].getHeight();

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
	for (int i = 0; i < NUM_LISTS; i++)
	{
		m_list[i].render(*globalUtilObjs->spriteBatch);
	}
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

void ShopScreen::OnBuyItemItem(const IOnBuyItemListener::Data& data)
{
	m_list[LIST_SELECTED_ITEM].addItem(new UIListItem_SelectedItem(&m_list[LIST_SELECTED_ITEM], *data.BoughtItem->Avatar));
}