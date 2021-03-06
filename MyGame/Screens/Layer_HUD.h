
#pragma once

#include <MyUIList.h>
#include "MiniMap.h"
#include "PlayerInfo.h"
#include "EventListener.h"

class Layer_HUD : public IOnPressListener, public OnPressListenee, public IOnBuyItemListener, public IOnPressListItemListener
{
public:
	struct InitBundle
	{
		MyVec3 MapCenter;
		MyVec2 MapSize;
		Hero* Player;
	};

	struct RenderBundle
	{
	};

private:
	// Assets
	enum
	{
		TEXTURE_MINIMAP_BACKGROUND,
		TEXTURE_MINIMAP_BTN_CLOSE,
		TEXTURE_MINIMAP_PLAYER,
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
		LIST_ITEM,
		LIST_SKILL,
		NUM_LISTS,
	};

	enum
	{
		LABEL_FPS,
		NUM_LABELS,
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
	void OnBuyItemItem(const IOnBuyItemListener::Data& data);
	void OnPressListItem(const IOnPressListItemListener::Data& data);

private:
	// Assets
	Texture m_textures[NUM_TEXTURES];
	Font    m_fonts[NUM_FONTS];

	// UI Widgets
	UIList  m_list[NUM_LISTS];
	UILabel m_labels[NUM_LABELS];

	// Other HUD-components
	MiniMap m_miniMap;
	PlayerInfo m_playerInfo;

	Hero* m_player;
};