
#include "MenuScreen.h"
#include <MyScreenManager.h>
#include <FrmPackedResourceGLES.h>

MenuScreen::MenuScreen(ScreenManager* screenManager)
	: Screen(screenManager)
{
}

MenuScreen::~MenuScreen()
{
}

void MenuScreen::init()
{
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/GUI.pak").c_str());

		m_texture_btn_startGame.init(resource.GetTexture("btn_start_game"));
	}

	m_btn_startGame.init(MyVec2(100, 100), m_texture_btn_startGame);
}

void MenuScreen::resize(int width, int height)
{

}

void MenuScreen::update(UserInput& userInput, Timer& timer)
{
	m_btn_startGame.update(userInput);
	if (m_btn_startGame.isPressing())
	{
		m_screenManager->activeScreen("PlayScreen");
	}
}

void MenuScreen::render(SpriteBatch& spriteBatch)
{
	m_btn_startGame.render(spriteBatch);
}