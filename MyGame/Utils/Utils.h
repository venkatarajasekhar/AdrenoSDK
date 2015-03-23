
#pragma once

// Always including MyGameServer.h on top because MyGameServer.h includes stl
#include <MyGameServer.h>

#include <MyTimer.h>
#include <MyInput.h>
#include <MySpriteBatch.h>

struct GLOBAL_UTIL_OBJS
{
	Timer* timer;
	UserInput* userInput;
	SpriteBatch* spriteBatch;
	GameServer* gameServer;
};