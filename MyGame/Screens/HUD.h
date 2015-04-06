
#pragma once

#include "MiniMap.h"

// HUD consists of:
//	- Minimap
//	- Fighting button

class HUD : public IOnPressListener, public OnPressListenee
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

	// Core functions
	void init(const MyVec3& mapCenter, const MyVec2& mapSize, IOnPressListener* listener);
	void resize(int width, int height);
	void update(Timer& timer, UserInput& userInput);
	void render(SpriteBatch& spriteBatch);

	// Event handling
	void OnPress(const IOnPressListener::Data& data);

private:
	// Assets
	Texture m_textures[NUM_TEXTURES];

	// Elements of HUD
	MiniMap m_miniMap;
	UIButton m_btn_fighting;
};