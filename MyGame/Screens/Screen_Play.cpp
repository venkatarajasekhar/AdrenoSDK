
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
		Layer_HUD::InitBundle bundle;
		bundle.MapCenter = MAP_CENTER;
		bundle.MapSize = MAP_SIZE;
		bundle.OpenShopListener = this;

		m_layer_HUD.init(bundle);
		m_layer_HUD.addPressListener(this);
	}

	{
		Layer_World::InitBundle bundle;
		bundle.MapCenter = MAP_CENTER;
		bundle.MapSize = MAP_SIZE;
		bundle.GameOverListener = this;

		m_layer_World.init(bundle);
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
		bundle.PlayerPos = MyVec3(0);

		m_layer_HUD.render(*globalUtilObjs->spriteBatch, bundle);
	}
}

void PlayScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "hud")
	{
		m_lockedUserInput = true;
	}
	else if (data.Id == "hud_btn_fighting")
	{
		m_screenManager->activeScreen("ShopScreen");
	}
}

void PlayScreen::OnGameOver(const IOnGameOverListener::Data& data)
{
	m_screenManager->activeScreen("GameOverScreen");
}