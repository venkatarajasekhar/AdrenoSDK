
#pragma once

#include <MyScreen.h>

class GameOverScreen : public Screen
{
public:
	GameOverScreen(ScreenManager* screenManager);
	~GameOverScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

private:

};