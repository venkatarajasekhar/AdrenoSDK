#pragma once

#include <MySpriteBatch.h>
#include "BloodBar.h"

class HeroInfo
{
public:
	HeroInfo();
	~HeroInfo();
	void init(Texture& heroIcon, Texture& goldIcon, Texture& foreground, Texture& background, 
		float health, float maxHealth,
		float mana, float maxMana,
		float exp, float maxExp);
	void render(SpriteBatch& spriteBatch, Camera& camera);
private:
	Texture* m_heroIcon;
	Texture* m_goldIcon;
	BloodBar m_healthBar;
	BloodBar m_manaBar;
	BloodBar m_expBar;

	float m_health;
	float m_maxHealth;
	float m_mana;
	float m_maxMana;
	float m_exp;
	float m_maxExp;
};