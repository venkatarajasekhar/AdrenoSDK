
#include "PlayerInfo.h"

//====================================================================================================================
//
// Constants
//
//====================================================================================================================

static const MyVec2 FIRST_BAR_POS = MyVec2(50.0f, 14.0f);
static const float BAR_DIST = 14.0f;
static const float GOLD_ICON_VMARGIN = 150.0f;

//====================================================================================================================
//
// PlayerInfo class
//
//====================================================================================================================

PlayerInfo::PlayerInfo()
{
}

PlayerInfo::~PlayerInfo()
{
}

void PlayerInfo::init(Font& font)
{
	// Assets textures
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/player_info.pak").c_str());

		m_textures[TEXTURE_EXP_BAR_BACK].init(resource.GetTexture("exp_bar_background"));
		m_textures[TEXTURE_EXP_BAR_FORE].init(resource.GetTexture("exp_bar_foreground"));
		m_textures[TEXTURE_HEALTH_BAR_BACK].init(resource.GetTexture("health_bar_background"));
		m_textures[TEXTURE_HEALTH_BAR_FORE].init(resource.GetTexture("health_bar_foreground"));
		m_textures[TEXTURE_MANA_BAR_BACK].init(resource.GetTexture("mana_bar_background"));
		m_textures[TEXTURE_MANA_BAR_FORE].init(resource.GetTexture("mana_bar_foreground"));

		m_textures[TEXTURE_EXP_ICON].init(resource.GetTexture("experience_icon"));
		m_textures[TEXTURE_HEALTH_ICON].init(resource.GetTexture("health_icon"));
		m_textures[TEXTURE_MANA_ICON].init(resource.GetTexture("mana_icon"));
		m_textures[TEXTURE_GOLD_ICON].init(resource.GetTexture("gold_icon"));
	}

	// Progress bar widgets
	m_progBars[PROG_BAR_HEALTH].init("", MyVec2(), m_textures[TEXTURE_HEALTH_BAR_FORE], m_textures[TEXTURE_HEALTH_BAR_BACK]);
	m_progBars[PROG_BAR_MANA].init("", MyVec2(), m_textures[TEXTURE_MANA_BAR_FORE], m_textures[TEXTURE_MANA_BAR_BACK]);
	m_progBars[PROG_BAR_EXP].init("", MyVec2(), m_textures[TEXTURE_EXP_BAR_FORE], m_textures[TEXTURE_EXP_BAR_BACK]);

	m_progBars[PROG_BAR_MANA].setProgress(0.2f);
	m_progBars[PROG_BAR_EXP].setProgress(0.9f);

	// Label widgets
	m_labels[LABEL_HEALTH].init("", MyVec2(), font, "");
	m_labels[LABEL_MANA].init("", MyVec2(), font, "40/4000");
	m_labels[LABEL_EXP].init("", MyVec2(), font, "1/50");
	m_labels[LABEL_GOLD].init("", MyVec2(), font, "");
}

void PlayerInfo::update(Timer& timer, UserInput& userInput)
{
	for (int i = 0; i < NUM_PROG_BARS; i++)
	{
		m_progBars[i].update(userInput);
	}

	for (int i = 0; i < NUM_LABELS; i++)
	{
		m_labels[i].update(userInput);
	}
}


void PlayerInfo::render(SpriteBatch& spriteBatch, Hero& player)
{
	{
		int health = player.getHealth(), maxHealth = player.getMaxHealth();
		m_labels[LABEL_HEALTH].setText(toString(health) + "/" + toString(maxHealth));
		m_progBars[PROG_BAR_HEALTH].setProgress((float)health / (float)maxHealth);

		int mana = player.getMana(), maxMana = player.getMaxMana();
		m_labels[LABEL_MANA].setText(toString(mana) + "/" + toString(maxMana));
		m_progBars[PROG_BAR_MANA].setProgress((float)mana / (float)maxMana);

		int exp = player.getExp(), maxExp = player.getMaxExp();
		m_labels[LABEL_EXP].setText(toString(exp) + "/" + toString(maxExp));
		m_progBars[PROG_BAR_EXP].setProgress((float)exp / (float)maxExp);
		
		m_labels[LABEL_GOLD].setText(toString(player.getGold()));
	}
	
	{
		MyVec2 pos = FIRST_BAR_POS;

		m_progBars[PROG_BAR_HEALTH].setPos(pos);
		m_progBars[PROG_BAR_HEALTH].render(spriteBatch);
		spriteBatch.renderTexture2D(
			(m_textures + TEXTURE_HEALTH_ICON), 
			pos - MyVec2(m_textures[TEXTURE_HEALTH_ICON].getWidth() - 7.0f, 8.0f));
		m_labels[LABEL_HEALTH].setPos(pos + 0.5f * (m_progBars[PROG_BAR_HEALTH].getSize() - m_labels[LABEL_HEALTH].getSize()));
		m_labels[LABEL_HEALTH].render(spriteBatch);

		pos.y += m_progBars[PROG_BAR_HEALTH].getSize().y + BAR_DIST;

		m_progBars[PROG_BAR_MANA].setPos(pos);
		m_progBars[PROG_BAR_MANA].render(spriteBatch);
		spriteBatch.renderTexture2D(
			(m_textures + TEXTURE_MANA_ICON), 
			pos - MyVec2(m_textures[TEXTURE_MANA_ICON].getWidth() - 8.0f, 10.0f));
		m_labels[LABEL_MANA].setPos(pos + 0.5f * (m_progBars[PROG_BAR_MANA].getSize() - m_labels[LABEL_MANA].getSize()));
		m_labels[LABEL_MANA].render(spriteBatch);

		pos.y += m_progBars[PROG_BAR_MANA].getSize().y + BAR_DIST;

		m_progBars[PROG_BAR_EXP].setPos(pos);
		m_progBars[PROG_BAR_EXP].render(spriteBatch);
		spriteBatch.renderTexture2D(
			(m_textures + TEXTURE_EXP_ICON), 
			pos - MyVec2(m_textures[TEXTURE_EXP_ICON].getWidth() - 8.0f, 8.0f));
		m_labels[LABEL_EXP].setPos(pos + 0.5f * (m_progBars[PROG_BAR_EXP].getSize() - m_labels[LABEL_EXP].getSize()));
		m_labels[LABEL_EXP].render(spriteBatch);

		pos.x = GOLD_ICON_VMARGIN;
		pos.y += m_progBars[PROG_BAR_EXP].getSize().y + BAR_DIST / 2.0f;
		spriteBatch.renderTexture2D((m_textures + TEXTURE_GOLD_ICON), pos);

		pos.x += m_textures[TEXTURE_GOLD_ICON].getWidth() + 5.0f;
		m_labels[LABEL_GOLD].setPos(pos);
		m_labels[LABEL_GOLD].render(spriteBatch);
	}
}