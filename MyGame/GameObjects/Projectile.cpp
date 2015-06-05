
#include "Projectile.h"

//==============================================================================================================
//
// Constants
//
//==============================================================================================================

static const float PROJECTILE_MOVING_SPEED = 15.0f;
static const float DISTANCE_THRESHOLD = 0.5f;
static const MyVec3 ATTACKER_OFFSET = MyVec3(0, 5, 0);
static const MyVec3 ATTACK_TARGET_OFFSET = MyVec3(0, 3, 0);

//==============================================================================================================
//
// Projectile class
//
//==============================================================================================================

Projectile::Projectile()
	: m_billboard(nullptr),
	m_inUse(false),
	m_attacker(nullptr),
	m_atkTarget(nullptr)
{
}

Projectile::~Projectile()
{
}

void Projectile::init()
{
}

void Projectile::update(Timer& timer)
{
	if ((m_attacker != nullptr) && (m_atkTarget != nullptr))
	{
		if (!m_atkTarget->inUse())
		{
			m_inUse = false;
		}
		else
		{
			if (distance_optimized(m_position, m_target) <= DISTANCE_THRESHOLD)
			{
				m_inUse = false;
				m_atkTarget->accHealth(-m_attacker->getDamage());
			}
			else
			{
				setTarget(m_atkTarget->getPos());
			}
		}
	}

	// Moving elements
	MyVec3 heading = PROJECTILE_MOVING_SPEED * normalize_optimized(m_target - m_position);
	m_position += heading * timer.getElapsedTime();
}

void Projectile::render(Camera& camera)
{
	if (m_billboard != nullptr)
	{
		m_billboard->setPos(m_position);
		m_billboard->render(camera);
	}
}

void Projectile::respawn(Billboard& billboard, LivingEntity* attacker, LivingEntity* atkTarget, MyVec3 attackerOffset)
{
	m_billboard = &billboard;
	setPos(attacker->getPos() + attackerOffset);
	setTarget(atkTarget->getPos());
	m_inUse = true;
	m_attacker = attacker;
	m_atkTarget = atkTarget;
}

// Getter

bool Projectile::inUse()const
{
	return m_inUse;
}

// Setter

void Projectile::setPos(const MyVec3& pos)
{
	m_position = pos + ATTACKER_OFFSET;
}

void Projectile::setTarget(const MyVec3& target)
{
	m_target = target + ATTACK_TARGET_OFFSET;
}

//==============================================================================================================
//
// ProjectilePool class
//
//==============================================================================================================

ProjectilePool::ProjectilePool()
{
}

ProjectilePool::~ProjectilePool()
{
}

void ProjectilePool::init()
{
	for (int i = 0; i < MAX_NUM_PROJECTILES; i++)
	{
		Projectile* projectile = m_projectiles + i;
		projectile->init();
	}
}

void ProjectilePool::update(Timer& timer)
{
	for (int i = 0; i < MAX_NUM_PROJECTILES; i++)
	{
		Projectile* projectile = m_projectiles + i;
		if (projectile->inUse())
		{
			projectile->update(timer);
		}
	}
}

void ProjectilePool::render(Camera& camera)
{
	for (int i = 0; i < MAX_NUM_PROJECTILES; i++)
	{
		Projectile* projectile = m_projectiles + i;
		if (projectile->inUse())
		{
			projectile->render(camera);
		}
	}
}

void ProjectilePool::spawnProjectile(Billboard& billboard, LivingEntity* attacker, LivingEntity* atkTarget, MyVec3 attackerOffset)
{
	Projectile* projectile = getFreeSlot();
	if (projectile != nullptr)
	{
		projectile->respawn(billboard, attacker, atkTarget, attackerOffset);
	}
}

Projectile* ProjectilePool::getFreeSlot()
{
	for (int i = 0; i < MAX_NUM_PROJECTILES; i++)
	{
		Projectile* projectile = m_projectiles + i;
		if (!projectile->inUse())
		{
			return projectile;
		}
	}

	return nullptr;
}