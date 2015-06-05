
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
Texture MenuScreen::m_textures[NUM_TEXTURES];
Font    MenuScreen::m_fonts[NUM_FONTS];
Audio   MenuScreen::m_audios[NUM_AUDIOS];

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

			m_textures[TEXTURE_COMMON_BACKGROUND].init(resource.GetTexture("menu_common_background"));
			m_textures[TEXTURE_COMMON_HPANEL].init(resource.GetTexture("menu_common_hpanel"));
			m_textures[TEXTURE_COMMON_VPANEL].init(resource.GetTexture("menu_common_vpanel"));
			m_textures[TEXTURE_COMMON_BTN_BACKGROUND].init(resource.GetTexture("menu_common_btn_background"));

			m_textures[TEXTURE_GAME_MODE_BTN_ONLINE].init(resource.GetTexture("menu_game_mode_btn_online"));
			m_textures[TEXTURE_GAME_MODE_BTN_SOLO].init(resource.GetTexture("menu_game_mode_btn_solo"));
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