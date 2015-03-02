
#include "MyScreenManager.h"
#include "MyScreen.h"

ScreenManager::ScreenManager()
	: m_activeScreen(nullptr)
{
}

ScreenManager::~ScreenManager()
{
	for (auto i = m_screens.begin(); i != m_screens.end(); ++i)
	{
		delete(i->second);
	}
	m_screens.clear();
}

void ScreenManager::resize(int width, int height)
{
	if (m_activeScreen != nullptr)
	{
		m_activeScreen->resize(width, height);
	}
}

void ScreenManager::update(UserInput& userInput, Timer& timer)
{
	if (m_activeScreen != nullptr)
	{
		m_activeScreen->update(userInput, timer);
	}
}

void ScreenManager::render(SpriteBatch& spriteBatch)
{
	if (m_activeScreen != nullptr)
	{
		m_activeScreen->render(spriteBatch);
	}
}

void ScreenManager::addScreen(const MyString& id, Screen* screen)
{
	if (getScreen(id) == nullptr)
	{
		struct { GLint x, y, width, height; } viewport;
		glGetIntegerv(GL_VIEWPORT, (GLint*)&viewport);

		screen->init();
		screen->resize(viewport.width, viewport.height);
		
		m_screens[id] = screen;
	}
}

void ScreenManager::activeScreen(const MyString& id)
{
	m_activeScreen = getScreen(id);
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