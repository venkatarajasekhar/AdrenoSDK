
#pragma once

#include "Tower.h"

class Tower_Main : public Tower, public OnGameOverListenee
{
public:
	Tower_Main();
	~Tower_Main();

private:
	void dead();
};