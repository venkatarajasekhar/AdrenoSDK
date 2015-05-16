
#include "MyScreen.h"

Screen::Screen(ScreenManager* screenManager)
	: m_screenManager(screenManager),
	m_tag(nullptr)
{
}

Screen::~Screen()
{
}

void Screen::setTag(void* tag)
{
	m_tag = tag;
}