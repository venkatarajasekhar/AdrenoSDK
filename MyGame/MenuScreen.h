
#pragma once

#include <MyScreen.h>

#include <MyUIButton.h>

class MenuScreen : public Screen
{
public:
	MenuScreen(ScreenManager* screenManager);
	~MenuScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

private:

	Texture m_texture_btn_startGame;

	UIButton m_btn_startGame;
};