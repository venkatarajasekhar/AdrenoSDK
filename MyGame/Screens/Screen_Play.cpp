
// Always including Utils.h on top
#include "Utils.h"

#include "Screen_Play.h"
#include <MyScreenManager.h>

//========================================================================================================
//
// Constants
//
//========================================================================================================

static const MyVec3 MAP_CENTER = MyVec3(0);
static const MyVec2 MAP_SIZE   = MyVec2(100);

//========================================================================================================
//
// PlayScreen class
//
//========================================================================================================

PlayScreen::PlayScreen(ScreenManager* screenManager)
	: Screen(screenManager),
	m_lockedUserInput(false)
{
}

PlayScreen::~PlayScreen()
{
}

void PlayScreen::init()
{
	{
		Layer_World::InitBundle bundle;
		bundle.MapCenter = MAP_CENTER;
		bundle.MapSize = MAP_SIZE;
		bundle.GameOverListener = this;
		bundle.ShopListener = this;

		m_layer_World.init(bundle);
	}

	{
		Layer_HUD::InitBundle bundle;
		bundle.MapCenter = MAP_CENTER;
		bundle.MapSize = MAP_SIZE;
		bundle.Player = m_layer_World.getPlayer();

		m_layer_HUD.init(bundle);
		m_layer_HUD.addPressListener(this);
	}
}

void PlayScreen::resize(int width, int height)
{
	m_layer_HUD.resize(width, height);
	m_layer_World.resize(width, height);
}

void PlayScreen::update(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	m_layer_HUD.update(*globalUtilObjs->timer, *globalUtilObjs->userInput);

	if (m_lockedUserInput)
	{
		globalUtilObjs->userInput->lock();
	}

	m_layer_World.update(*globalUtilObjs->timer, *globalUtilObjs->userInput);

	globalUtilObjs->userInput->unlock();
	m_lockedUserInput = false;
}

void PlayScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	m_layer_World.render(*globalUtilObjs->spriteBatch);

	{
		Layer_HUD::RenderBundle bundle;

		m_layer_HUD.render(*globalUtilObjs->spriteBatch, bundle);
	}
}

void PlayScreen::beginActive()
{
	m_layer_World.beginActive();
}

void PlayScreen::lockUserInput(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	globalUtilObjs->userInput->lock();
}

void PlayScreen::unlockUserInput(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	globalUtilObjs->userInput->unlock();
}

void PlayScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "hud")
	{
		m_lockedUserInput = true;
	}
	else if (data.Id == "shop")
	{
		std::vector<IOnBuyItemListener* > listener;
		listener.push_back(m_layer_World.getPlayer());
		listener.push_back(&m_layer_HUD);

		m_screenManager->activePopupScreen("ShopScreen", &listener);
	}
}

void PlayScreen::OnGameOver(const IOnGameOverListener::Data& data)
{
	TEAM_TYPE winner = data.WinnerTeam;

	m_layer_World.onGameOver();
	m_screenManager->activeScreen("GameOverScreen", &winner);
}