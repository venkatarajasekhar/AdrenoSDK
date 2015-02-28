
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

private:
	CFrmTimer m_frmTimer;
};