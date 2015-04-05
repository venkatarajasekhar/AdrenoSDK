#include "Global.h"
#include "Projectile.h"


Projectile::Projectile()
	: m_billboard(nullptr)
{
}

Projectile::~Projectile()
{
}

void Projectile::init(Billboard& billboard,
	int idEnemy,
	int idHero,
	float range,
	TEAM_TYPE teamType)
{
	m_billboard = &billboard;
	m_idEnemy = idEnemy;
	m_idHero = idHero;
	m_range = range;
	m_teamType = teamType;
	m_entityType = ENTITY_TYPE_PROJECTILE;
	m_active = true;

	MyVec3 dir(dSin(g_livingEntityManager.getLivingEntityById(m_idHero)->getInstance()->Rotation.y), 0, dCos(g_livingEntityManager.getLivingEntityById(m_idHero)->getInstance()->Rotation.y));
	MyVec3 offset = normalize(dir) + MyVec3(0, 3, 0);
	MyVec3 pos = g_livingEntityManager.getLivingEntityById(m_idHero)->getInstance()->Position + 0.5f*offset;
	MyVec3 posTarget = g_livingEntityManager.getLivingEntityById(m_idEnemy)->getInstance()->Position;
	//m_movingEntity.setPos(g_livingEntityManager.getLivingEntityById(m_idHero)->getInstance()->Position + 0.5f*offset);
	//m_movingEntity.setTarget(g_livingEntityManager.getLivingEntityById(m_idEnemy)->getInstance()->Position);
	m_movingEntity.init(pos, posTarget, MyVec3(0), 0, 5, 180);
}

void Projectile::update(Timer& timer)
{
	if (m_active)
	{ 
		if (g_livingEntityManager.getLivingEntityById(m_idEnemy) == NULL)
		{
			m_active = false;
		}
		else
		{
			//if (!m_movingEntity.isMoving())

			if (distance_optimized(getPos(), g_livingEntityManager.getLivingEntityById(m_idEnemy)->getInstance()->Position) <= 2.0f)
			{
				m_active = false;
				g_livingEntityManager.getLivingEntityById(m_idEnemy)->setHealth(g_livingEntityManager.getLivingEntityById(m_idEnemy)->getHealth()
					- g_livingEntityManager.getLivingEntityById(m_idHero)->getDamage() * 3);
			}
			setTarget(g_livingEntityManager.getLivingEntityById(m_idEnemy)->getInstance()->Position);
			m_movingEntity.update(timer);
		}
	}
}

void Projectile::render(Camera& camera)
{
	if (m_active)
	{
		m_billboard->setPos(m_movingEntity.getPos());
		m_billboard->render(camera);
	}
}

// Setter
void Projectile::setPos(const MyVec3& pos)
{
	m_movingEntity.setPos(pos);
}

MyVec3 Projectile::getPos()
{
	return m_movingEntity.getPos();
}

int Projectile::getIdEnemy()
{
	return m_idEnemy;
}

int Projectile::getIdHero()
{
	return m_idHero;
}

float Projectile::getRange()
{
	return m_range;
}

void Projectile::setTarget(const MyVec3& target)
{
	m_movingEntity.setTarget(target);
}

void Projectile::setActive(bool active)
{
	m_active = active;
}

bool Projectile::getActive()
{
	return m_active;
}