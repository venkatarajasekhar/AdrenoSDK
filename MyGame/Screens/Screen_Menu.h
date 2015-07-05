
#pragma once

#include <MyScreen.h>

#include <MyFont.h>
#include <MyAudio.h>
#include <MySpriteSheet.h>

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

		// Buy hero menu
		TEXTURE_BUY_HERO_DIAMOND_BAR,
		TEXTURE_BUY_HERO_BTN_BUY,
		TEXTURE_BUY_HERO_FIGHTER_DANMEI,
		TEXTURE_BUY_HERO_FIGHTER_ELRATH,
		TEXTURE_BUY_HERO_FIGHTER_TIAC,
		TEXTURE_BUY_HERO_FIGHTER_VALLE,
		TEXTURE_BUY_HERO_GUARDIAN_DICK,
		TEXTURE_BUY_HERO_GUARDIAN_SEWON,
		TEXTURE_BUY_HERO_MAGE_CEZANNE,
		TEXTURE_BUY_HERO_SHOOTER_RENOAH,

		NUM_TEXTURES,
	};

	enum
	{
		SPRITE_SHEET_MAIN_TITLE,
		NUM_SPRITE_SHEETS,
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
	static Texture     m_textures[NUM_TEXTURES];
	static SpriteSheet m_spriteSheets[NUM_SPRITE_SHEETS];
	static Font        m_fonts[NUM_FONTS];
	static Audio       m_audios[NUM_AUDIOS];
};
