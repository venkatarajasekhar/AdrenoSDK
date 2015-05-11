
#pragma once

#include <MyScreen.h>
#include <MyUIButton.h>
#include <MyUIList.h>

struct ItemInfo
{
	MyString Name;
	MyString Desc;
	int Price;
	MyString Benefit;
	Texture* Avatar;
};

class ShopScreen : public Screen, public IOnPressListener, public IOnPressListItemListener
{
private:
	static const int NUM_ITEMS = 6;

private:
	// Assets
	enum
	{
		// Background panel
		TEXTURE_SHOP_BACKGROUND,
		TEXTURE_LIST_ITEM_BACKGROUND,
		TEXTURE_DESC_ITEM_BACKGROUND,
		TEXTURE_ITEM_BACKGROUND,

		// Button
		TEXTURE_SHOP_BTN_CLOSE,
		TEXTURE_SHOP_BTN_BACKGROUND,
		
		// Icon
		TEXTURE_GOLD_ICON,
		
		// Item avatar
		TEXTURE_ITEM_BLOOD_POUCH,
		TEXTURE_ITEM_CHAIN_MAIL,
		TEXTURE_ITEM_CLOAK_OF_THE_RESISTANT,
		TEXTURE_ITEM_LIFE_WARD,
		TEXTURE_ITEM_LIGHT_CALVARY_HAT,
		TEXTURE_ITEM_STAFF_OF_SATHLENAR,
		NUM_TEXTURES,
	};

	enum
	{
		FONT_CONSOLAS_12,
		NUM_FONTS,
	};

	// UI Widgets
	enum
	{
		BTN_CLOSE,
		NUM_BTNS,
	};

	enum
	{
		LABEL_ITEM_DESC,
		NUM_LABELS,
	};

public:
	ShopScreen(ScreenManager* screenManager);
	~ShopScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

	void OnPress(const IOnPressListener::Data& data);
	void OnPressListItem(const IOnPressListItemListener::Data& data);

private:
	void initItemInfo();

private:
	int m_width, m_height;

	// Assets
	Texture m_textures[NUM_TEXTURES];
	Font    m_fonts[NUM_FONTS];

	// UI Widgets
	UIImageButton m_btns[NUM_BTNS];
	UILabel m_labels[NUM_LABELS];
	UIList m_shopItemList;

	ItemInfo m_itemInfo[NUM_ITEMS];
};