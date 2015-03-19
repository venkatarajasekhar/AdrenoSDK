
#pragma once

#include "MyUtils.h"

class Timer
{
public:
	Timer();
	~Timer();

	void update();

	float getElapsedTime();
	float getTotalTime();
	float getFPS();

private:
	CFrmTimer m_frmTimer;
};