
#pragma once

#include <MyScreen.h>
#include "Layer_HUD.h"
#include "Layer_World.h"

class PlayScreen : public Screen, public IOnPressListener, public IOnGameOverListener
{
public:
	PlayScreen(ScreenManager* screenManager);
	~PlayScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

	void OnPress(const IOnPressListener::Data& data);
	void OnGameOver(const IOnGameOverListener::Data& data);

private:
	Layer_HUD   m_layer_HUD;
	Layer_World m_layer_World;

	bool m_lockedUserInput;
};