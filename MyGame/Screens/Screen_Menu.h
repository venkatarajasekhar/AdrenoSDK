
#pragma once

#include <MyScreen.h>

#include <MyUIButton.h>
#include <MyFont.h>

class MenuScreen : public Screen, public IOnPressListener
{
private:
	// Assets
	enum
	{
		TEXTURE_BTN_BACKGROUND,
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
		BTN_START_GAME,
		BTN_SIGN_IN,
		BTN_SIGN_OUT,
		BTN_ACHIEVEMENTS,
		BTN_LEADERBOARD,
		NUM_BTNS,
	};

public:
	MenuScreen(ScreenManager* screenManager);
	~MenuScreen();

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
	UITextButton m_btns[NUM_BTNS];
};