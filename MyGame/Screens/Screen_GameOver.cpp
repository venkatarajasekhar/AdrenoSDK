
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_GameOver.h"

GameOverScreen::GameOverScreen(ScreenManager* screenManager)
	: Screen(screenManager),
	m_width(0),
	m_height(0)
{
}

GameOverScreen::~GameOverScreen()
{
}

void GameOverScreen::init()
{
	m_font.init(resolveAssetsPath("Fonts/Consolas12.pak"));
}

void GameOverScreen::resize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void GameOverScreen::update(void* utilObjs)
{

}

void GameOverScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	{
		MyString text("You won !!!");

		MyVec2 pos(
			(m_width - m_font.getTextWidth(text)) / 2, 
			(m_height - m_font.getTextHeight()) / 2
			);

		globalUtilObjs->spriteBatch->renderText2D(m_font, text, pos);
	}
}