
#pragma once

#include <MyMiniMap.h>

class HUD
{
private:
	enum
	{
		TEXTURE_MINIMAP_BACKGROUND,
		TEXTURE_MINIMAP_BTN_CLOSE,
		TEXTURE_MINIMAP_PLAYER,
		TEXTURE_BTN_FIGHTING,
		NUM_TEXTURES,
	};

public:
	HUD();
	~HUD();

	void init();
	void resize(int width, int height);
	void update(Timer& timer, UserInput& userInput, bool& isClicked);
	void render(SpriteBatch& spriteBatch);

private:
	Texture m_textures[NUM_TEXTURES];

	MiniMap m_miniMap;
	UIButton m_btn_fighting;
};