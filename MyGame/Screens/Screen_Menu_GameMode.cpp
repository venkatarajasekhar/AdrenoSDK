
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_Menu_GameMode.h"
#include <MyScreenManager.h>

GameModeMenuScreen::GameModeMenuScreen(ScreenManager* screenManager)
	: MenuScreen(screenManager)
{
}

GameModeMenuScreen::~GameModeMenuScreen()
{
}

void GameModeMenuScreen::init()
{
	MenuScreen::init();

	// Buttons
	m_imageBtns[IMAGE_BTN_PLAY_ONLINE].init("btn_play_online", MyVec2(), m_textures[TEXTURE_GAME_MODE_BTN_ONLINE]);
	m_imageBtns[IMAGE_BTN_PLAY_SOLO].init("btn_play_solo", MyVec2(200, 0), m_textures[TEXTURE_GAME_MODE_BTN_SOLO]);

	for (size_t i = 0; i < NUM_IMAGE_BTNS; i++)
	{
		m_imageBtns[i].addPressListener(this);
	}
}

void GameModeMenuScreen::resize(int width, int height)
{
	MenuScreen::resize(width, height);
}

void GameModeMenuScreen::update(void* utilObjs)
{
	MenuScreen::update(utilObjs);

	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Buttons
	for (int i = 0; i < NUM_IMAGE_BTNS; i++)
	{
		m_imageBtns[i].update(*globalUtilObjs->userInput);
	}
}

void GameModeMenuScreen::render(void* utilObjs)
{
	MenuScreen::render(utilObjs);

	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Buttons
	for (int i = 0; i < NUM_IMAGE_BTNS; i++)
	{
		m_imageBtns[i].render(*globalUtilObjs->spriteBatch);
	}
}

void GameModeMenuScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "btn_play_online")
	{

	}
	else if (data.Id == "btn_play_solo")
	{
		m_screenManager->activeScreen("PlayScreen");
	}
}