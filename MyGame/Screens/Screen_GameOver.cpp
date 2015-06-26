
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_GameOver.h"
#include <MyScreenManager.h>
#include "BaseEntity.h"

//===============================================================================================================
//
// Constants
//
//===============================================================================================================

//===============================================================================================================
//
// GameOverScreen class
//
//===============================================================================================================

GameOverScreen::GameOverScreen(ScreenManager* screenManager)
	: MenuScreen(screenManager)
{
}

GameOverScreen::~GameOverScreen()
{
}

void GameOverScreen::init()
{
	MenuScreen::init();

	MyColor textColor = MyColor(212.0f / 255.0f, 170.0f / 225.0f, 0.0f / 255.0f);

	// Buttons
	m_btns[BTN_BACK_TO_MEMU].init(
		"btn_back_to_menu", 
		MyVec2(), 
		m_textures[TEXTURE_COMMON_BTN_BACKGROUND], 
		"MAIN MENU", 
		m_fonts[FONT_BAUHAUS93_26],
		textColor,
		0.7f);

	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].addPressListener(this);
	}

	// Labels
	m_labels[LABEL_COMBAT_RESULT].init("", MyVec2(), m_fonts[FONT_BAUHAUS93_26], "", textColor);
}

void GameOverScreen::resize(int width, int height)
{
	MenuScreen::resize(width, height);

	changeUIPos();
}

void GameOverScreen::update(void* utilObjs)
{
	MenuScreen::update(utilObjs);

	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Buttons
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].update(*globalUtilObjs->userInput);
	}
}

void GameOverScreen::render(void* utilObjs)
{
	MenuScreen::render(utilObjs);

	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Labels
	for (int i = 0; i < NUM_LABELS; i++)
	{
		m_labels[i].render(*globalUtilObjs->spriteBatch);
	}

	// Buttons
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].render(*globalUtilObjs->spriteBatch);
	}
}

void GameOverScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "btn_back_to_menu")
	{
		m_screenManager->activeScreen("MainMenuScreen");
	}
}

void GameOverScreen::setTag(void* tag)
{
	if (tag != nullptr)
	{
		TEAM_TYPE winner = *((TEAM_TYPE*)tag);

		if (winner == TEAM_TYPE_MY_TEAM)
		{
			m_labels[LABEL_COMBAT_RESULT].setText("Victory...");
		}
		else
		{
			m_labels[LABEL_COMBAT_RESULT].setText("Defeated...");
		}

		changeUIPos();
	}
}

void GameOverScreen::changeUIPos()
{
	// Labels
	{
		MyVec2 pos(
			(m_width - m_labels[LABEL_COMBAT_RESULT].getSize().x) / 2,
			(m_height - m_labels[LABEL_COMBAT_RESULT].getSize().y) / 2
			);

		m_labels[LABEL_COMBAT_RESULT].setPos(pos);
	}

	// Buttons
	{
		float yOffset = 0.5f * (m_labels[LABEL_COMBAT_RESULT].getPos().y - m_btns[BTN_BACK_TO_MEMU].getSize().y);
		MyVec2 pos(
			(m_width - m_btns[BTN_BACK_TO_MEMU].getSize().x) / 2,
			m_labels[LABEL_COMBAT_RESULT].getPos().y + m_labels[LABEL_COMBAT_RESULT].getSize().y + yOffset
			);

		m_btns[BTN_BACK_TO_MEMU].setPos(pos);
	}
}