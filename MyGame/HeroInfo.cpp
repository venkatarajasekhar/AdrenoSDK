#include "HeroInfo.h"

HeroInfo::HeroInfo()
{

}

HeroInfo::~HeroInfo()
{

}

void HeroInfo::init(Texture& heroIcon, Texture& goldIcon, Texture& foreground, Texture& background,
	float health, float maxHealth,
	float mana, float maxMana,
	float exp, float maxExp)
{
	m_heroIcon = &heroIcon;
	m_goldIcon = &goldIcon;
	m_health = health;
	m_maxHealth = maxHealth;
	m_mana = mana;
	m_maxMana = maxMana;
	m_exp = exp;
	m_maxExp = maxExp;
}

void HeroInfo::render(SpriteBatch& spriteBatch, Camera& camera)
{
	spriteBatch.renderTexture2D(m_heroIcon, MyVec2(10, 20), 0, MyVec2(1, 1));
	spriteBatch.renderTexture2D(m_goldIcon, MyVec2(20, 10), 0, MyVec2(1, 1));
}