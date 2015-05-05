
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
		TEXTURE_DUMP_LIST_BACKGROUND,
		TEXTURE_DUMP_LIST_ITEM_1,
		TEXTURE_DUMP_LIST_ITEM_2,
		TEXTURE_DUMP_LIST_ITEM_3,
		TEXTURE_DUMP_LIST_ITEM_4,
		TEXTURE_DUMP_LIST_ITEM_5,
		TEXTURE_DUMP_LIST_ITEM_6,
		NUM_TEXTURES,
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

	// UI Widgets
	UIButton m_btns[NUM_BTNS];
	UIList m_dumpList;
};