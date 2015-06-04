
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_Menu.h"
#include <MyScreenManager.h>
#include <FrmPackedResourceGLES.h>

//=================================================================================================================================
//
// Constants
//
//=================================================================================================================================

static const float BTN_CONTAINER_HMARGIN = 30.0f;
static const float BTN_VDIST = 10.0f;

//=================================================================================================================================
//
// MenuScreen class
//
//=================================================================================================================================

MenuScreen::MenuScreen(ScreenManager* screenManager)
	: Screen(screenManager),
	m_width(0),
	m_height(0)
{
}

MenuScreen::~MenuScreen()
{
}

void MenuScreen::init()
{
	// Texture assets
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/gui_menu.pak").c_str());

		m_textures[TEXTURE_MENU_BACKGROUND].init(resource.GetTexture("menu_background"));
		m_textures[TEXTURE_BTN_CONTAINER].init(resource.GetTexture("btn_container"));
		m_textures[TEXTURE_BTN_BACKGROUND].init(resource.GetTexture("menu_btn_background"));
	}

	m_btnContainerDest.Size = MyVec2(m_textures[TEXTURE_BTN_CONTAINER].getWidth(), m_textures[TEXTURE_BTN_CONTAINER].getHeight());

	// Font assets
	m_fonts[FONT_BAUHAUS93_26].init(resolveAssetsPath("Fonts/Bauhaus9326.pak"));

	// Audio assets
	m_audios[AUDIO_WELCOME].init(resolveAssetsPath("Audios/Welcome.wav"));
	m_audios[AUDIO_GOODBYE].init(resolveAssetsPath("Audios/GoodBye.wav"));
	m_audios[AUDIO_WIN].init(resolveAssetsPath("Audios/Win.wav"), true);
	m_audios[AUDIO_WIN].setVolume(0.3f);
	m_audios[AUDIO_WIN].play();

	// Buttons
	{
		MyColor textColor = MyColor(212.0f / 255.0f, 170.0f / 225.0f, 0.0f / 255.0f);

		m_btns[BTN_START_GAME].init(
			"btn_menu_start_game",
			MyVec2(0),
			m_textures[TEXTURE_BTN_BACKGROUND],
			"START",
			m_fonts[FONT_BAUHAUS93_26],
			textColor);
		m_btns[BTN_SIGN_IN].init(
			"btn_menu_sign_in",
			MyVec2(0),
			m_textures[TEXTURE_BTN_BACKGROUND],
			"Sign-In",
			m_fonts[FONT_BAUHAUS93_26],
			textColor);
		m_btns[BTN_SIGN_OUT].init(
			"btn_menu_sign_out",
			MyVec2(0),
			m_textures[TEXTURE_BTN_BACKGROUND],
			"Sign-Out",
			m_fonts[FONT_BAUHAUS93_26],
			textColor);
		m_btns[BTN_ACHIEVEMENTS].init(
			"btn_menu_achievements",
			MyVec2(0),
			m_textures[TEXTURE_BTN_BACKGROUND],
			"Achievements",
			m_fonts[FONT_BAUHAUS93_26],
			textColor);
		m_btns[BTN_LEADERBOARD].init(
			"btn_menu_leaderboard",
			MyVec2(0),
			m_textures[TEXTURE_BTN_BACKGROUND],
			"Leaderboard",
			m_fonts[FONT_BAUHAUS93_26],
			textColor);
	}
	
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].addPressListener(this);
	}
}

void MenuScreen::resize(int width, int height)
{
	m_width = width;
	m_height = height;

	// Button container
	m_btnContainerDest.Pos = MyVec2(
		m_width - BTN_CONTAINER_HMARGIN - m_btnContainerDest.Size.x,
		0.5f * (m_height - m_btnContainerDest.Size.y));

	// Buttons
	{
		float btns_height = BTN_VDIST * (NUM_BTNS - 1);

		for (int i = 0; i < NUM_BTNS; i++)
		{
			btns_height += m_btns[i].getSize().y;
		}

		MyVec2 pos = MyVec2(0, m_btnContainerDest.Pos.y + 0.5f * (m_btnContainerDest.Size.y - btns_height));

		for (int i = 0; i < NUM_BTNS; i++)
		{
			pos.x = m_btnContainerDest.Pos.x + 0.5f * (m_btnContainerDest.Size.x - m_btns[i].getSize().x);

			m_btns[i].setPos(pos);

			pos.y += m_btns[i].getSize().y + BTN_VDIST;
		}
	}
}

void MenuScreen::update(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Buttons
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].update(*globalUtilObjs->userInput);
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

		globalUtilObjs->spriteBatch->renderTexture2D(&m_textures[TEXTURE_MENU_BACKGROUND], dest);
		globalUtilObjs->spriteBatch->renderTexture2D(&m_textures[TEXTURE_BTN_CONTAINER], m_btnContainerDest);
	}
	
	// Buttons
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].render(*globalUtilObjs->spriteBatch);
	}
}

void MenuScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "btn_menu_start_game")
	{
		m_audios[AUDIO_WIN].stop();
		m_screenManager->activeScreen("PlayScreen");
	}
	else if (data.Id == "btn_menu_sign_in")
	{
		m_audios[AUDIO_WELCOME].play();
		//globalUtilObjs->gameServer->beginUserInitiatedSignIn();
	}
	else if (data.Id == "btn_menu_sign_out")
	{
		m_audios[AUDIO_GOODBYE].play();
		//globalUtilObjs->gameServer->signOut();
	}
	else if (data.Id == "btn_menu_achievements")
	{
		//globalUtilObjs->gameServer->showAchievements();
	}
	else if (data.Id == "btn_menu_leaderboard")
	{

	}
}