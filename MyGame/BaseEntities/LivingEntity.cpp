
#include "LivingEntity.h"
#include <MyUtils.h>

/*
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
/**/

LivingEntity::LivingEntity()
	: m_maxHealth(0),
	m_health(0),
	m_damage(0),
	m_bloodBar(nullptr),
	m_lEnts(nullptr),
	m_atkTarget(nullptr),
	m_atkRange(0)
{
}

LivingEntity::~LivingEntity()
{
}

void LivingEntity::init(
	int maxHealth,
	int damage,
	BloodBar& bloodBar,
	const MyVec2& bloodBarScale,
	const MyVec3& bloodBarOffset,
	std::vector<LivingEntity*>& lEnts,
	float atkRange)
{
	m_maxHealth = maxHealth;
	m_health = m_maxHealth;
	m_damage = damage;

	m_bloodBar = &bloodBar;
	m_bloodBarScale = bloodBarScale;
	m_bloodBarOffset = bloodBarOffset;

	m_lEnts = &lEnts;
	m_atkRange = atkRange;
}

void LivingEntity::render(SpriteBatch& spriteBatch, Camera& camera, Light& light)
{
	m_bloodBar->setScale(m_bloodBarScale);
	m_bloodBar->render(spriteBatch, camera, getPos() + m_bloodBarOffset, (float)m_health / (float)m_maxHealth);
}

// Getter

int LivingEntity::getHealth()const
{
	return m_health;
}

// Setter

void LivingEntity::accMaxHealth(int delta)
{
	m_maxHealth += delta;
	if (m_maxHealth < 0)
	{
		m_maxHealth = 0;
	}
}

void LivingEntity::accHealth(int delta)
{
	m_health += delta;
	m_health = clamp(m_health, 0, m_maxHealth);
}

void LivingEntity::accDamage(int delta)
{
	m_damage += delta;
	if (m_damage < 0)
	{
		m_damage = 0;
	}
}