
#pragma once

#include <MyScreen.h>

#include <MyUIButton.h>
#include <MyFont.h>
#include <MyAudio.h>

class MenuScreen : public Screen, public IOnPressListener
{
private:
	// Assets
	enum
	{
		TEXTURE_MENU_BACKGROUND,
		TEXTURE_BTN_CONTAINER,
		TEXTURE_BTN_BACKGROUND,
		NUM_TEXTURES,
	};

	enum
	{
		FONT_BAUHAUS93_26,
		NUM_FONTS,
	};

	enum
	{
		AUDIO_WELCOME,
		AUDIO_GOODBYE,
		AUDIO_WIN,
		NUM_AUDIOS,
	};

	// UI Widgets
	enum
	{
		BTN_PLAY_GAME,
		BTN_HEROES,
		BTN_ACHIEVEMENTS,
		BTN_LEADERBOARD,
		BTN_SETTINGS,
		BTN_PROFILE,
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
	Audio   m_audios[NUM_AUDIOS];

	// UI Widgets
	UITextButton m_btns[NUM_BTNS];

	Rect2D m_btnContainerDest;
};