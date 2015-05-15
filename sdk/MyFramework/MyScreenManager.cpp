
#include "MyScreenManager.h"
#include "MyScreen.h"

ScreenManager::ScreenManager()
	: m_activeScreen(nullptr),
	m_popupScreen(nullptr),
	m_justActiveScreen(false),
	m_emptyTex(nullptr)
{
}

ScreenManager::~ScreenManager()
{
	for (auto i = m_screens.begin(); i != m_screens.end(); ++i)
	{
		delete(i->second);
	}
}

//===========================================================================================================
//
// Core
//
//===========================================================================================================

void ScreenManager::init(Texture& emptyTex)
{
	m_emptyTex = &emptyTex;
}

static void resizeScr(Screen* scr, int width, int height)
{
	if (scr != nullptr)
	{
		scr->resize(width, height);
	}
}

void ScreenManager::resize(int width, int height)
{
	resizeScr(m_activeScreen, width, height);
	resizeScr(m_popupScreen, width, height);
}

static void updateScr(Screen* scr, void* utilObjs)
{
	if (scr != nullptr)
	{
		scr->update(utilObjs);
	}
}

void ScreenManager::update(void* utilObjs)
{
	if (m_popupScreen != nullptr)
	{
		m_activeScreen->lockUserInput(utilObjs);
	}

	updateScr(m_activeScreen, utilObjs);

	if (m_popupScreen != nullptr)
	{
		m_activeScreen->unlockUserInput(utilObjs);
	}

	updateScr(m_popupScreen, utilObjs);

	m_justActiveScreen = false;
}

static void renderScr(Screen* scr, void* utilObjs, bool justActiveScreen)
{
	if (scr != nullptr && !justActiveScreen)
	{
		scr->render(utilObjs);
	}
}

void ScreenManager::render(void* utilObjs)
{
	renderScr(m_activeScreen, utilObjs, m_justActiveScreen);
	renderScr(m_popupScreen, utilObjs, m_justActiveScreen);
}

//===========================================================================================================
//
// Active screens
//
//===========================================================================================================

void ScreenManager::activeScreen(const MyString& id)
{
	m_activeScreen = getScreen(id);

	if (m_activeScreen != nullptr)
	{
		int width, height;
		getWindowDimension(width, height);

		m_activeScreen->resize(width, height);

		m_justActiveScreen = true;
	}
}

void ScreenManager::activePopupScreen(const MyString& id)
{
	m_popupScreen = getScreen(id);

	if (m_popupScreen != nullptr)
	{
		int width, height;
		getWindowDimension(width, height);

		m_popupScreen->resize(width, height);

		m_justActiveScreen = true;
	}
}

void ScreenManager::deactivePopupScreen()
{
	m_popupScreen = nullptr;
}

//===========================================================================================================
//
// Misc
//
//===========================================================================================================

void ScreenManager::addScreen(const MyString& id, Screen* screen)
{
	if (getScreen(id) == nullptr)
	{
		int width, height;
		getWindowDimension(width, height);

		screen->init();
		screen->resize(width, height);

		m_screens[id] = screen;
	}
}

Screen* ScreenManager::getScreen(const MyString& id)
{
	auto i = m_screens.find(id);
	if (i != m_screens.end())
	{
		return i->second;
	}
	else
	{
		return nullptr;
	}
}