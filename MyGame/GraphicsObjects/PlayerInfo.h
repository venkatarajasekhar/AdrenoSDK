
#pragma once

#include <MyUILabel.h>
#include <MyUIProgressBar.h>
#include "Hero.h"

class PlayerInfo
{
private:
	// Assets
	enum
	{
		TEXTURE_EXP_BAR_BACK,
		TEXTURE_EXP_BAR_FORE,
		TEXTURE_HEALTH_BAR_BACK,
		TEXTURE_HEALTH_BAR_FORE,
		TEXTURE_MANA_BAR_BACK,
		TEXTURE_MANA_BAR_FORE,
		TEXTURE_EXP_ICON,
		TEXTURE_HEALTH_ICON,
		TEXTURE_MANA_ICON,
		TEXTURE_GOLD_ICON,
		NUM_TEXTURES,
	};

	// UI Widgets
	enum
	{
		PROG_BAR_HEALTH,
		PROG_BAR_MANA,
		PROG_BAR_EXP,
		NUM_PROG_BARS,
	};

	enum
	{
		LABEL_HEALTH,
		LABEL_MANA,
		LABEL_EXP,
		LABEL_GOLD,
		NUM_LABELS,
	};

public:
	PlayerInfo();
	~PlayerInfo();

	void init(Font& font);
	void update(Timer& timer, UserInput& userInput);
	void render(SpriteBatch& spriteBatch, Hero& player);

private:
	// Assets
	Texture m_textures[NUM_TEXTURES];

	// UI Widgets
	UILabel m_labels[NUM_LABELS];
	UIProgressBar m_progBars[NUM_PROG_BARS];
};