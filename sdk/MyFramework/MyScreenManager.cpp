
#include "MyScreenManager.h"
#include "MyScreen.h"

ScreenManager::ScreenManager()
	: m_activeScreen(nullptr),
	m_justActiveScreen(false)
{
}

ScreenManager::~ScreenManager()
{
	for (auto i = m_screens.begin(); i != m_screens.end(); ++i)
	{
		delete(i->second);
	}
}

void ScreenManager::resize(int width, int height)
{
	if (m_activeScreen != nullptr)
	{
		m_activeScreen->resize(width, height);
	}
}

void ScreenManager::update(void* utilObjs)
{
	if (m_activeScreen != nullptr)
	{
		m_justActiveScreen = false;
		m_activeScreen->update(utilObjs);
	}
}

void ScreenManager::render(void* utilObjs)
{
	if (m_activeScreen != nullptr && !m_justActiveScreen)
	{
		m_activeScreen->render(utilObjs);
	}
}

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