
#include "LivingEntity.h"

int LivingEntity::m_numIDs = 0;

LivingEntity::LivingEntity()
	: m_maxHealth(0),
	m_health(0),
	m_damage(0)
{
	m_idEntity = m_numIDs++;
}

LivingEntity::~LivingEntity()
{
}

void LivingEntity::init(int maxHealth, int damage, float range, float radius, ENTITY_TYPE entityType, TEAM_TYPE teamType)
{
	m_maxHealth = maxHealth;
	m_health = m_maxHealth;
	m_damage = damage;
	m_range = range;
	m_radius = radius;
	m_entityType = entityType;
	m_teamType = teamType;
}

int LivingEntity::getHealth()const
{
	return m_health;
}

void LivingEntity::setHealth(int health)
{
	m_health = health;
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

int LivingEntity::getIdEntity()const
{
	return m_idEntity;
}

float LivingEntity::getRange()const
{
	return m_range;
}

float LivingEntity::getRadius()const
{
	return m_radius;
}