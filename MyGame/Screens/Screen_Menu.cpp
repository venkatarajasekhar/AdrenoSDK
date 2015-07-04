
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_Menu.h"
#include <MyScreenManager.h>
#include <FrmPackedResourceGLES.h>

//=================================================================================================================================
//
// MenuScreen class
//
//=================================================================================================================================

bool MenuScreen::m_initedStatic = false;
int MenuScreen::m_width = 0; 
int MenuScreen::m_height = 0;

// Assets
Texture     MenuScreen::m_textures[NUM_TEXTURES];
SpriteSheet MenuScreen::m_spriteSheets[NUM_SPRITE_SHEETS];
Font        MenuScreen::m_fonts[NUM_FONTS];
Audio       MenuScreen::m_audios[NUM_AUDIOS];

MenuScreen::MenuScreen(ScreenManager* screenManager)
	: Screen(screenManager)
{
}

MenuScreen::~MenuScreen()
{
}

void MenuScreen::init()
{
	if (!m_initedStatic)
	{
		// Texture assets
		{
			CFrmPackedResourceGLES resource;
			resource.LoadFromFile(resolveAssetsPath("Textures/gui_menu.pak").c_str());

			// Texture
			m_textures[TEXTURE_COMMON_BACKGROUND].init(resource.GetTexture("menu_common_background"));
			m_textures[TEXTURE_COMMON_HPANEL].init(resource.GetTexture("menu_common_hpanel"));
			m_textures[TEXTURE_COMMON_VPANEL].init(resource.GetTexture("menu_common_vpanel"));
			m_textures[TEXTURE_COMMON_BTN_BACKGROUND].init(resource.GetTexture("menu_common_btn_background"));

			m_textures[TEXTURE_GAME_MODE_BTN_ONLINE].init(resource.GetTexture("menu_game_mode_btn_online"));
			m_textures[TEXTURE_GAME_MODE_BTN_SOLO].init(resource.GetTexture("menu_game_mode_btn_solo"));

			m_textures[TEXTURE_BUY_HERO_DIAMOND_BAR].init(resource.GetTexture("menu_buy_hero_diamond_bar"));
			m_textures[TEXTURE_BUY_HERO_BTN_BUY].init(resource.GetTexture("menu_buy_hero_btn_buy"));
			m_textures[TEXTURE_BUY_HERO_FIGHTER_DANMEI].init(resource.GetTexture("menu_buy_hero_Fighter_DanMei"));
			m_textures[TEXTURE_BUY_HERO_FIGHTER_ELRATH].init(resource.GetTexture("menu_buy_hero_Fighter_Elrath"));
			m_textures[TEXTURE_BUY_HERO_FIGHTER_TIAC].init(resource.GetTexture("menu_buy_hero_Fighter_Tiac"));
			m_textures[TEXTURE_BUY_HERO_FIGHTER_VALLE].init(resource.GetTexture("menu_buy_hero_Fighter_Valle"));
			m_textures[TEXTURE_BUY_HERO_GUARDIAN_DICK].init(resource.GetTexture("menu_buy_hero_Guardian_Dick"));
			m_textures[TEXTURE_BUY_HERO_GUARDIAN_SEWON].init(resource.GetTexture("menu_buy_hero_Guardian_Sewon"));
			m_textures[TEXTURE_BUY_HERO_MAGE_CEZANNE].init(resource.GetTexture("menu_buy_hero_Mage_Cezanne"));
			m_textures[TEXTURE_BUY_HERO_SHOOTER_RENOAH].init(resource.GetTexture("menu_buy_hero_Shooter_Renoah"));

			// Sprite sheet
			m_spriteSheets[SPRITE_SHEET_MAIN_TITLE].init(resource.GetTexture("menu_main_game_title"), 10, MyIVec2(5, 1), MyIVec2(395, 406));
		}

		// Font assets
		m_fonts[FONT_BAUHAUS93_26].init(resolveAssetsPath("Fonts/Bauhaus9326.pak"));

		// Audio assets
		m_audios[AUDIO_WELCOME].init(resolveAssetsPath("Audios/Welcome.wav"));
		m_audios[AUDIO_GOODBYE].init(resolveAssetsPath("Audios/GoodBye.wav"));
		m_audios[AUDIO_WIN].init(resolveAssetsPath("Audios/Win.wav"), true);
		m_audios[AUDIO_WIN].setVolume(0.3f);
		m_audios[AUDIO_WIN].play();

		m_initedStatic = true;
	}
}

void MenuScreen::resize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void MenuScreen::update(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	for (int i = 0; i < NUM_SPRITE_SHEETS; i++)
	{
		m_spriteSheets[i].update(*globalUtilObjs->timer);
	}
}

void MenuScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Background
	{
		Rect2D dest;
		dest.Pos = MyVec2(0, 0);
		dest.Size = MyVec2(m_width, m_height);

		globalUtilObjs->spriteBatch->renderTexture2D(&m_textures[TEXTURE_COMMON_BACKGROUND], dest);
	}
}