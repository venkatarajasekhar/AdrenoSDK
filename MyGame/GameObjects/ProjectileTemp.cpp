#include "Global.h"
#include "Projectile.h"


Projectile::Projectile()
	: m_billboard(nullptr)
{
}

Projectile::~Projectile()
{
}

void Projectile::init(Billboard& billboard, PROJECTILE_TYPE projectileType, std::vector<LivingEntity*>& lEnts)
{
	m_billboard = &billboard;
	m_projectileType = projectileType;
	m_lEnts = &lEnts;
	m_active = false;
}

/*void Projectile::init(Billboard& billboard,
	int idEnemy,
	int idHero,
	float range,
	int damage,
	TEAM_TYPE teamType)
{
	m_billboard = &billboard;
	m_idEnemy = idEnemy;
	m_idHero = idHero;
	m_range = range;
	m_damage = damage;
	m_teamType = teamType;
	m_entityType = ENTITY_TYPE_PROJECTILE;
	m_active = true;

	MyVec3 dir(dSin(g_livingEntityManager.getLivingEntityById(m_idHero)->getInstance()->Rotation.y), 0, dCos(g_livingEntityManager.getLivingEntityById(m_idHero)->getInstance()->Rotation.y));
	MyVec3 offset = normalize(dir) + MyVec3(0, 3, 0);
	MyVec3 pos = g_livingEntityManager.getLivingEntityById(m_idHero)->getInstance()->Position + 0.5f*offset;
	MyVec3 posTarget = g_livingEntityManager.getLivingEntityById(m_idEnemy)->getInstance()->Position;

	m_movingEntity.init(pos, posTarget, MyVec3(0), 0, 8, 10000);
}*/

void Projectile::update(Timer& timer)
{
	if (m_active)
	{ 
		if (!m_enemy->inUse())
		{
			m_active = false;
		}
		else
		{
			if (distance_optimized(getPos(), m_enemy->getPos()) <= 2.0f)
			{
				m_active = false;
				m_enemy->accHealth(-m_hero->getDamage());
			}
			setTarget(m_enemy->getPos());
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

PROJECTILE_TYPE Projectile::getProjectileType() const
{
	return m_projectileType;
}

//=========================================================================================================
//
// ProjectilePool class
//
//=========================================================================================================

ProjectilePool::ProjectilePool()
{
}

ProjectilePool::~ProjectilePool()
{
}

void ProjectilePool::init(Shader& billboardShader, std::vector<LivingEntity*>& lEnts)
{
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/sprite_sheet.pak").c_str());

		m_spriteSheets[SPRITESHEET_ENERGY_BALL].init(resource.GetTexture("energy_ball"), 5, MyIVec2(3, 1), MyIVec2(100, 100));
		m_spriteSheets[SPRITESHEET_FIRE_BALL].init(resource.GetTexture("fire_ball"), 5, MyIVec2(1, 1), MyIVec2(96, 32));
	}

	m_billboards[BILLBOARD_FIRE_BALL].init(&m_spriteSheets[SPRITESHEET_FIRE_BALL], billboardShader, MyVec3(0), MyVec2(1.2f, 0.4f), 0);
	m_billboards[BILLBOARD_ENERGY_BALL].init(&m_spriteSheets[SPRITESHEET_ENERGY_BALL], billboardShader, MyVec3(0), MyVec2(1.0f), 0);

	for (size_t i = 0; i < MAX_NUM_EACH_PROJECTILE; i++)
		m_projectiles[i].init(m_billboards[BILLBOARD_FIRE_BALL], PROJECTILE_FIRE_BALL, lEnts);
	for (size_t i = 0; i < MAX_NUM_EACH_PROJECTILE; i++)
		m_projectiles[i].init(m_billboards[BILLBOARD_ENERGY_BALL], PROJECTILE_ENERGY_BALL, lEnts);
}

void ProjectilePool::update(Timer& timer)
{

}

void ProjectilePool::render(Camera& camera, Light& light)
{

}

Projectile* ProjectilePool::getFreeSlot(Projectile* container, int size, PROJECTILE_TYPE type)
{
	for (int i = 0; i < MAX_NUM_EACH_PROJECTILE * NUM_BILLBOARD; i++)
	{
		if ((!m_projectiles[i].getActive()) && (m_projectiles[i].getProjectileType() == type))
			return &m_projectiles[i];
	}

	return nullptr;
}
