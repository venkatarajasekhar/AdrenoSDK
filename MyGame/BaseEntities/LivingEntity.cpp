
#include "LivingEntity.h"
#include <MyUtils.h>

LivingEntity::LivingEntity()
	: m_maxHealth(0),
	m_health(0),
	m_damage(0),
	m_bloodBar(nullptr),
	m_lEnts(nullptr),
	m_atkTarget(nullptr),
	m_atkRange(0),
	m_inUse(false),
	m_selectedDecal(nullptr),
	m_selectedRadius(2.0f),
	m_selected(false)
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
	float atkRange,
	Quad3D* selectedDecal)
{
	m_maxHealth = maxHealth;
	m_health = m_maxHealth;
	m_damage = damage;

	m_bloodBar = &bloodBar;
	m_bloodBarScale = bloodBarScale;
	m_bloodBarOffset = bloodBarOffset;

	m_lEnts = &lEnts;
	m_atkRange = atkRange;
	
	m_selectedDecal = selectedDecal;
}

void LivingEntity::render(SpriteBatch& spriteBatch, Camera& camera, Light& light)
{
	// Render blood bar
	m_bloodBar->setScale(m_bloodBarScale);
	m_bloodBar->render(spriteBatch, camera, getPos() + m_bloodBarOffset, (float)m_health / (float)m_maxHealth);

	// Render selection indicator
	if (m_selected && (m_selectedDecal != nullptr))
	{
		m_selectedDecal->setPos(getPos() + MyVec3(0, 0.1f, 0));
		m_selectedDecal->setSize(MyVec2(2.0f * m_selectedRadius));
		m_selectedDecal->render(camera);
	}
}

void LivingEntity::dead()
{
	m_inUse = false;
}

// Getter

int LivingEntity::getMaxHealth()const
{
	return m_maxHealth;
}

int LivingEntity::getHealth()const
{
	return m_health;
}

int LivingEntity::getDamage()const
{
	return m_damage;
}

bool LivingEntity::inUse()const
{
	return m_inUse;
}

bool LivingEntity::isSelect(bool isPressed, MyVec3& pressedPoint)
{
	return (isPressed && (distance_optimized(getPos(), pressedPoint) <= m_selectedRadius));
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

	if (m_health == 0)
	{
		dead();
	}
}

void LivingEntity::accDamage(int delta)
{
	m_damage += delta;
	if (m_damage < 0)
	{
		m_damage = 0;
	}
}

void LivingEntity::select()
{
	m_selected = true;
}

void LivingEntity::deselect()
{
	m_selected = false;
}