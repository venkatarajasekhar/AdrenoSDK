
// Always including Utils.h on top
#include "Utils.h"

#include "MenuScreen.h"
#include <MyScreenManager.h>
#include <FrmPackedResourceGLES.h>

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
	// Texture resources
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/GUI.pak").c_str());

		m_texture_btns[0].init(resource.GetTexture("btn_start_game"));
		m_texture_btns[1].init(resource.GetTexture("btn_sign_in"));
		m_texture_btns[2].init(resource.GetTexture("btn_sign_out"));
		m_texture_btns[3].init(resource.GetTexture("btn_achievements"));
		m_texture_btns[4].init(resource.GetTexture("btn_leaderboard"));
	}

	// Buttons
	for (int i = 0; i < NUM_BUTTONS; i++)
	{
		m_btns[i].init(MyVec2(0), m_texture_btns[i]);
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

	// Buttons
	for (int i = 0; i < NUM_BUTTONS; i++)
	{
		m_btns[i].update(*globalUtilObjs->userInput);
	}

	if (m_btns[0].isPressing())
	{
		m_screenManager->activeScreen("PlayScreen");
	}

	if (m_btns[1].isPressing())
	{
		globalUtilObjs->gameServer->beginUserInitiatedSignIn();
	}

	if (m_btns[2].isPressing())
	{
		globalUtilObjs->gameServer->signOut();
	}

	if (m_btns[3].isPressing())
	{
		globalUtilObjs->gameServer->showAchievements();
	}

	if (m_btns[4].isPressing())
	{
	}
}

void MenuScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Buttons
	int vDis = 10;
	int btns_height = vDis * (NUM_BUTTONS - 1);

	for (int i = 0; i < NUM_BUTTONS; i++)
	{
		btns_height += m_texture_btns[i].getHeight();
	}

	int yCoor = (m_height - btns_height) / 2;

	for (int i = 0; i < NUM_BUTTONS; i++)
	{
		int xCoor = (m_width - m_texture_btns[i].getWidth()) / 2;

		m_btns[i].setPos(MyVec2(xCoor, yCoor));
		m_btns[i].render(*globalUtilObjs->spriteBatch);

		yCoor += m_texture_btns[i].getHeight() + vDis;
	}
}