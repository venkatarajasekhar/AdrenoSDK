
#pragma once

#include <MyTimer.h>
#include <MyInput.h>
#include <MySpriteBatch.h>
#include <MyGameServer.h>

struct GLOBAL_UTIL_OBJS
{
	Timer* timer;
	UserInput* userInput;
	SpriteBatch* spriteBatch;
	GameServer* gameServer;
};