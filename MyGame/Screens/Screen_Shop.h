
#pragma once

#include <MyScreen.h>
#include <MyUIButton.h>
#include <MyUIList.h>

class ShopScreen : public Screen, public IOnPressListener
{
private:
	// Assets
	enum
	{
		TEXTURE_SHOP_BACKGROUND,
		TEXTURE_SHOP_BTN_CLOSE,
		TEXTURE_LIST_ITEM_BACKGROUND,
		TEXTURE_ITEM_BACKGROUND,
		TEXTURE_GOLD_ICON,
		TEXTURE_SHOP_BTN_BACKGROUND,
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

public:
	ShopScreen(ScreenManager* screenManager);
	~ShopScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

	void OnPress(const IOnPressListener::Data& data);

private:
	int m_width, m_height;

	// Assets
	Texture m_textures[NUM_TEXTURES];
	Font    m_fonts[NUM_FONTS];

	// UI Widgets
	UIImageButton m_btns[NUM_BTNS];
	UIList m_shopItemList;
};