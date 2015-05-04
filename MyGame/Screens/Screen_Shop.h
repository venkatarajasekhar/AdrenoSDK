
#pragma once

#include <MyScreen.h>
#include <MyUIButton.h>

class ShopScreen : public Screen, public IOnPressListener
{
private:
	// Assets
	enum
	{
		TEXTURE_SHOP_BACKGROUND,
		TEXTURE_SHOP_BTN_CLOSE,
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
};