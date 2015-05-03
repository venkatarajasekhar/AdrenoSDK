
#pragma once

#include "MiniMap.h"

class Layer_HUD : public IOnPressListener, public OnPressListenee
{
public:
	struct InitBundle
	{
		MyVec3 MapCenter;
		MyVec2 MapSize;
		IOnPressListener* OpenShopListener;
	};

	struct RenderBundle
	{
		MyVec3 PlayerPos;
	};

private:
	// Assets
	enum
	{
		TEXTURE_MINIMAP_BACKGROUND,
		TEXTURE_MINIMAP_BTN_CLOSE,
		TEXTURE_MINIMAP_PLAYER,
		TEXTURE_BTN_FIGHTING,
		NUM_TEXTURES,
	};

	enum
	{
		FONT_CONSOLAS_12,
		NUM_FONTS,
	};

	// UI Widgets
	enum
	{
		BTN_FIGHTING,
		NUM_BTNS,
	};

public:
	Layer_HUD();
	~Layer_HUD();

	// Core functions

	void init(Layer_HUD::InitBundle& bundle);
	void resize(int width, int height);
	void update(Timer& timer, UserInput& userInput);
	void render(SpriteBatch& spriteBatch, Layer_HUD::RenderBundle& bundle);

	// Event handling

	void OnPress(const IOnPressListener::Data& data);

private:
	// Assets
	Texture m_textures[NUM_TEXTURES];
	Font    m_fonts[NUM_FONTS];

	// UI Widgets
	UIButton m_btns[NUM_BTNS];

	// Other HUD-components
	MiniMap m_miniMap;
	float m_fps;
};