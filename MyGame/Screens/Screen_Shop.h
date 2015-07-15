
#pragma once

#include <MyScreen.h>
#include <MyUIButton.h>
#include <MyUIList.h>
#include "Hero.h"
#include "EventListener.h"
#include "Notify.h"

//===================================================================================================================
//
// ShopScreen class
//
//===================================================================================================================

class ShopScreen : 
	public Screen, 
	public IOnPressListener, 
	public IOnPressListItemListener, 
	public OnBuyItemListenee,
	public IOnBuyItemListener
{
private:
	static const int TOTAL_HERO_ITEMS = 14;

private:
	// Assets
	enum
	{
		// Background panel
		TEXTURE_SHOP_BACKGROUND,
		TEXTURE_LIST_ITEM_BACKGROUND,
		TEXTURE_LIST_SELECTED_ITEM_BACKGROUND,
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

		// Misc
		TEXTURE_EMPTY,

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

	enum
	{
		LIST_ITEM,
		LIST_SELECTED_ITEM,
		NUM_LISTS,
	};

	enum
	{
		AUDIO_ITEM_BUY,
		AUDIO_NOT_ENOUGH_MONEY,
		NUM_AUDIOS,
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
	void OnBuyItemItem(const IOnBuyItemListener::Data& data);

	void setTag(void* tag);

private:
	void initItems();

private:
	int m_width, m_height;

	// Assets
	Texture m_textures[NUM_TEXTURES];
	Font    m_fonts[NUM_FONTS];

	// UI Widgets
	UIImageButton m_btns[NUM_BTNS];
	UILabel m_labels[NUM_LABELS];
	UIList m_list[NUM_LISTS];

	HeroItem* m_totalItems[TOTAL_HERO_ITEMS];
	int m_numBoughtItems;

	Hero* m_player;

	Notify2D m_notify;

	Audio m_audios[NUM_AUDIOS];
};