
#include "MyTimer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::update()
{
	m_frmTimer.MarkFrame();
}

float Timer::getElapsedTime()
{
	return m_frmTimer.GetFrameElapsedTime();
}

float Timer::getTotalTime()
{
	return m_frmTimer.GetFrameTime();
}