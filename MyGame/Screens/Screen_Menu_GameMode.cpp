
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_Menu_GameMode.h"
#include "Screen_Play.h"
#include <MyScreenManager.h>

//=================================================================================================================================
//
// Constants
//
//=================================================================================================================================

static const float BTN_HDIST = 10.0f;

//=================================================================================================================================
//
// GameModeMenuScreen class
//
//=================================================================================================================================

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

	m_btnContainerDest.Size = MyVec2(m_textures[TEXTURE_COMMON_HPANEL].getWidth(), m_textures[TEXTURE_COMMON_HPANEL].getHeight());

	// Buttons
	m_imageBtns[IMAGE_BTN_PLAY_ONLINE].init("btn_play_online", MyVec2(), m_textures[TEXTURE_GAME_MODE_BTN_ONLINE]);
	m_imageBtns[IMAGE_BTN_PLAY_SOLO].init("btn_play_solo", MyVec2(), m_textures[TEXTURE_GAME_MODE_BTN_SOLO]);

	for (size_t i = 0; i < NUM_IMAGE_BTNS; i++)
	{
		m_imageBtns[i].addPressListener(this);
	}

	{
		MyColor textColor = MyColor(212.0f / 255.0f, 170.0f / 225.0f, 0.0f / 255.0f);

		m_textBtns[TEXT_BTN_BACK].init(
			"btn_back", 
			MyVec2(), 
			m_textures[TEXTURE_COMMON_BTN_BACKGROUND], 
			"BACK", 
			m_fonts[FONT_BAUHAUS93_26], 
			textColor);
	}

	for (int i = 0; i < NUM_TEXT_BTNS; i++)
	{
		m_textBtns[i].addPressListener(this);
	}
	
	// Labels
	{
		MyColor textColor = MyColor(212.0f / 255.0f, 170.0f / 225.0f, 0.0f / 255.0f);
		m_labels[LABEL_SCREEN_TITLE].init("", MyVec2(), m_fonts[FONT_BAUHAUS93_26], "GAME MODE", textColor);
	}
}

void GameModeMenuScreen::resize(int width, int height)
{
	MenuScreen::resize(width, height);

	// Button container
	m_btnContainerDest.Pos = MyVec2(
		0.5f * (m_width - m_btnContainerDest.Size.x),
		0.5f * (m_height - m_btnContainerDest.Size.y));

	// Choose game mode button
	{
		MyVec2 btnsSize = MyVec2(
			m_imageBtns[IMAGE_BTN_PLAY_SOLO].getSize().x + m_imageBtns[IMAGE_BTN_PLAY_ONLINE].getSize().x + BTN_HDIST,
			m_imageBtns[IMAGE_BTN_PLAY_SOLO].getSize().y);
		MyVec2 pos = m_btnContainerDest.Pos + 0.5f * (m_btnContainerDest.Size - btnsSize);

		m_imageBtns[IMAGE_BTN_PLAY_SOLO].setPos(pos);

		pos.x += m_imageBtns[IMAGE_BTN_PLAY_SOLO].getSize().x + BTN_HDIST;

		m_imageBtns[IMAGE_BTN_PLAY_ONLINE].setPos(pos);
	}

	// Back button
	{
		float yOffset = 0.5f * (m_btnContainerDest.Pos.y - m_textBtns[TEXT_BTN_BACK].getSize().y);
		MyVec2 pos = m_btnContainerDest.Pos + 
			MyVec2(
			0.5f * (m_btnContainerDest.Size.x - m_textBtns[TEXT_BTN_BACK].getSize().x),
			m_btnContainerDest.Size.y + yOffset);

		m_textBtns[TEXT_BTN_BACK].setPos(pos);
	}

	// Title
	{
		float yOffset = 0.5f * (m_btnContainerDest.Pos.y - m_labels[LABEL_SCREEN_TITLE].getSize().y);
		MyVec2 pos = MyVec2(
			m_btnContainerDest.Pos.x + 0.5f * (m_btnContainerDest.Size.x - m_labels[LABEL_SCREEN_TITLE].getSize().x),
			yOffset);

		m_labels[LABEL_SCREEN_TITLE].setPos(pos);
	}
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
	for (int i = 0; i < NUM_TEXT_BTNS; i++)
	{
		m_textBtns[i].update(*globalUtilObjs->userInput);
	}

	// Labels
	for (int i = 0; i < NUM_LABELS; i++)
	{
		m_labels[i].update(*globalUtilObjs->userInput);
	}
}

void GameModeMenuScreen::render(void* utilObjs)
{
	MenuScreen::render(utilObjs);

	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	globalUtilObjs->spriteBatch->renderTexture2D(&m_textures[TEXTURE_COMMON_HPANEL], m_btnContainerDest);

	// Buttons
	for (int i = 0; i < NUM_IMAGE_BTNS; i++)
	{
		m_imageBtns[i].render(*globalUtilObjs->spriteBatch);
	}
	for (int i = 0; i < NUM_TEXT_BTNS; i++)
	{
		m_textBtns[i].render(*globalUtilObjs->spriteBatch);
	}

	// Labels
	for (int i = 0; i < NUM_LABELS; i++)
	{
		m_labels[i].render(*globalUtilObjs->spriteBatch);
	}
}

void GameModeMenuScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "btn_play_online")
	{
	}
	else if (data.Id == "btn_play_solo")
	{
		//m_screenManager->addScreen("PlayScreen", new PlayScreen(m_screenManager), true);
		m_screenManager->activeScreen("PlayScreen");
	}
	else if (data.Id == "btn_back")
	{
		m_screenManager->activeScreen("MainMenuScreen");
	}
}