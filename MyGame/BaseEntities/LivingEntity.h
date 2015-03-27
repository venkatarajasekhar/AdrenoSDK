
#pragma once

#include "BaseEntity.h"

class LivingEntity : public BaseEntity
{
public:
	LivingEntity();
	virtual ~LivingEntity();

	virtual void init(int maxHealth, int damage);

	int getHealth()const;
	float getHealthRatio()const;
	int getDamage()const;
	bool isDead()const;

protected:
	int m_maxHealth;
	int m_health;
	int m_damage;
};