
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_Menu_Main.h"
#include <MyScreenManager.h>

//=================================================================================================================================
//
// Constants
//
//=================================================================================================================================

static const float BTN_CONTAINER_HMARGIN = 30.0f;
static const float BTN_VDIST = 1.0f;

//=================================================================================================================================
//
// MainMenuScreen class
//
//=================================================================================================================================

MainMenuScreen::MainMenuScreen(ScreenManager* screenManager)
	: MenuScreen(screenManager)
{
}

MainMenuScreen::~MainMenuScreen()
{
}

void MainMenuScreen::init()
{
	MenuScreen::init();

	m_btnContainerDest.Size = MyVec2(m_textures[TEXTURE_COMMON_VPANEL].getWidth(), m_textures[TEXTURE_COMMON_VPANEL].getHeight());

	// Buttons
	{
		MyColor textColor = MyColor(212.0f / 255.0f, 170.0f / 225.0f, 0.0f / 255.0f);

		m_btns[BTN_PLAY_GAME].init(
			"btn_menu_play_game",
			MyVec2(0),
			m_textures[TEXTURE_COMMON_BTN_BACKGROUND],
			"PLAY GAME",
			m_fonts[FONT_BAUHAUS93_26],
			textColor,
			0.8f);
		m_btns[BTN_HEROES].init(
			"btn_menu_heroes",
			MyVec2(0),
			m_textures[TEXTURE_COMMON_BTN_BACKGROUND],
			"HEROES",
			m_fonts[FONT_BAUHAUS93_26],
			textColor,
			0.9f);
		m_btns[BTN_ACHIEVEMENTS].init(
			"btn_menu_achievements",
			MyVec2(0),
			m_textures[TEXTURE_COMMON_BTN_BACKGROUND],
			"ACHIEVEMENTS",
			m_fonts[FONT_BAUHAUS93_26],
			textColor,
			0.7f);
		m_btns[BTN_LEADERBOARD].init(
			"btn_menu_leaderboard",
			MyVec2(0),
			m_textures[TEXTURE_COMMON_BTN_BACKGROUND],
			"LEADERBOARD",
			m_fonts[FONT_BAUHAUS93_26],
			textColor,
			0.7f);
		m_btns[BTN_SETTINGS].init(
			"btn_menu_settings",
			MyVec2(0),
			m_textures[TEXTURE_COMMON_BTN_BACKGROUND],
			"SETTINGS",
			m_fonts[FONT_BAUHAUS93_26],
			textColor,
			0.9f);
		m_btns[BTN_PROFILE].init(
			"btn_menu_profile",
			MyVec2(0),
			m_textures[TEXTURE_COMMON_BTN_BACKGROUND],
			"PROFILE",
			m_fonts[FONT_BAUHAUS93_26],
			textColor,
			0.9f);
	}

	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].addPressListener(this);
	}
}

void MainMenuScreen::resize(int width, int height)
{
	MenuScreen::resize(width, height);

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

void MainMenuScreen::update(void* utilObjs)
{
	MenuScreen::update(utilObjs);

	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Buttons
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].update(*globalUtilObjs->userInput);
	}
}

void MainMenuScreen::render(void* utilObjs)
{
	MenuScreen::render(utilObjs);

	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	globalUtilObjs->spriteBatch->renderTexture2D(&m_textures[TEXTURE_COMMON_VPANEL], m_btnContainerDest);

	// Buttons
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].render(*globalUtilObjs->spriteBatch);
	}
}

void MainMenuScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "btn_menu_play_game")
	{
		m_audios[AUDIO_WIN].stop();
		m_screenManager->activeScreen("GameModeMenuScreen");
	}
	else if (data.Id == "btn_menu_heroes")
	{
	}
	else if (data.Id == "btn_menu_achievements")
	{
		m_audios[AUDIO_WELCOME].play();
		//globalUtilObjs->gameServer->showAchievements();
	}
	else if (data.Id == "btn_menu_leaderboard")
	{
		m_audios[AUDIO_GOODBYE].play();
	}
	else if (data.Id == "btn_menu_settings")
	{
	}
	else if (data.Id == "btn_menu_profile")
	{

	}
}