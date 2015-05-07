
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_Menu.h"
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
	// Texture assets
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/gui_menu.pak").c_str());

		m_textures[TEXTURE_BTN_BACKGROUND].init(resource.GetTexture("menu_btn_background"));
	}

	// Fonts assets
	m_fonts[FONT_CONSOLAS_12].init(resolveAssetsPath("Fonts/Consolas24.pak"));

	// Buttons
	m_btns[BTN_START_GAME].init("btn_menu_start_game", MyVec2(0), m_textures[TEXTURE_BTN_BACKGROUND], "Start", m_fonts[FONT_CONSOLAS_12]);
	m_btns[BTN_SIGN_IN].init("btn_menu_sign_in", MyVec2(0), m_textures[TEXTURE_BTN_BACKGROUND], "Sign-In", m_fonts[FONT_CONSOLAS_12]);
	m_btns[BTN_SIGN_OUT].init("btn_menu_sign_out", MyVec2(0), m_textures[TEXTURE_BTN_BACKGROUND], "Sign-Out", m_fonts[FONT_CONSOLAS_12]);
	m_btns[BTN_ACHIEVEMENTS].init("btn_menu_achievements", MyVec2(0), m_textures[TEXTURE_BTN_BACKGROUND], "Achievements", m_fonts[FONT_CONSOLAS_12]);
	m_btns[BTN_LEADERBOARD].init("btn_menu_leaderboard", MyVec2(0), m_textures[TEXTURE_BTN_BACKGROUND], "Leaderboard", m_fonts[FONT_CONSOLAS_12]);

	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].addPressListener(this);
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
	for (int i = 0; i < NUM_BTNS; i++)
	{
		m_btns[i].update(*globalUtilObjs->userInput);
	}
}

void MenuScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Buttons
	int vDis = 10;
	int btns_height = vDis * (NUM_BTNS - 1);

	for (int i = 0; i < NUM_BTNS; i++)
	{
		btns_height += m_btns[i].getSize().y;
	}

	int yCoor = (m_height - btns_height) / 2;

	for (int i = 0; i < NUM_BTNS; i++)
	{
		int xCoor = (m_width - m_btns[i].getSize().x) / 2;

		m_btns[i].setPos(MyVec2(xCoor, yCoor));
		m_btns[i].render(*globalUtilObjs->spriteBatch);

		yCoor += m_btns[i].getSize().y + vDis;
	}
}

void MenuScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "btn_menu_start_game")
	{
		m_screenManager->activeScreen("PlayScreen");
	}
	else if (data.Id == "btn_menu_sign_in")
	{
		//globalUtilObjs->gameServer->beginUserInitiatedSignIn();
	}
	else if (data.Id == "btn_menu_sign_out")
	{
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