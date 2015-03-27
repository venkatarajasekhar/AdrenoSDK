
#include "LivingEntity.h"

LivingEntity::LivingEntity()
	: m_maxHealth(0),
	m_health(0),
	m_damage(0)
{
}

LivingEntity::~LivingEntity()
{
}

void LivingEntity::init(int maxHealth, int damage)
{
	m_maxHealth = maxHealth;
	m_health = m_maxHealth;
	m_damage = damage;
}

int LivingEntity::getHealth()const
{
	return m_health;
}

float LivingEntity::getHealthRatio()const
{
	return ((float)m_health / (float)m_maxHealth);
}

int LivingEntity::getDamage()const
{
	return m_damage;
}

bool LivingEntity::isDead()const
{
	return (m_health <= 0);
}