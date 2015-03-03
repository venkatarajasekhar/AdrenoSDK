
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
	static const int NUM_BUTTONS = 5;

private:
	int m_width, m_height;

	// Texture resources
	Texture m_texture_btns[NUM_BUTTONS];

	// Buttons
	UIButton m_btns[NUM_BUTTONS];
};