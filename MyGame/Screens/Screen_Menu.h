
#pragma once

#include <MyScreen.h>

#include <MyFont.h>
#include <MyAudio.h>

class MenuScreen : public Screen
{
protected:
	// Assets
	enum
	{
		// Common
		TEXTURE_COMMON_BACKGROUND,
		TEXTURE_COMMON_HPANEL,
		TEXTURE_COMMON_VPANEL,
		TEXTURE_COMMON_BTN_BACKGROUND,

		// Game mode menu
		TEXTURE_GAME_MODE_BTN_ONLINE,
		TEXTURE_GAME_MODE_BTN_SOLO,

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

public:
	MenuScreen(ScreenManager* screenManager);
	virtual ~MenuScreen();

	virtual void init();
	virtual void resize(int width, int height);
	virtual void update(void* utilObjs);
	virtual void render(void* utilObjs);

protected:
	static bool m_initedStatic;
	static int m_width, m_height;

	// Assets
	static Texture m_textures[NUM_TEXTURES];
	static Font    m_fonts[NUM_FONTS];
	static Audio   m_audios[NUM_AUDIOS];
};