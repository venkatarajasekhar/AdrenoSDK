#include "Tower.h"

void Tower::init(
	Adreno::Model* model,
	FileMesh1::MeshTextures& textures,
	Shader& shader,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	BloodBar* bloodBarRed,
	BloodBar* bloodBarGreen,
	Billboard& billboard,
	int health,
	int damage,
	float range,
	TEAM_TYPE teamType)
{
	LivingEntity::init(health, damage, range, 2.0f, ENTITY_TYPE_TOWER, teamType);

	{
		Material material;
		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		m_tower.init(model, textures.Textures, shader, &material);

		m_instance = FileMesh1::buildMeshInstance(pos, rot, scale);

		m_tower.addInstance(m_instance);
	}

	if (teamType == MY_TEAM) m_bloodBar = bloodBarGreen;
	else m_bloodBar = bloodBarRed;

	m_billboard = billboard;
}

void Tower::update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height)
{
	if (g_livingEntityManager.getIdLivingEntityInRange(m_idEntity, m_range) != -1)
	{
		if (m_countTime >= 3)
		{
			Projectile* m_projectile = new Projectile;
			m_idEmemy = g_livingEntityManager.getIdLivingEntityInRange(m_idEntity, m_range);
			m_projectile->init(m_billboard, m_idEmemy, m_idEntity, 10.0f, 40, MY_TEAM);
			g_projectileManager.insertProjectileToList(m_projectile);
			m_countTime -= 3;
		}
		m_countTime += timer.getElapsedTime();
	}
	else m_countTime = 3;

	m_tower.update(timer);
}

int Tower::findLivingEntityToBeat()
{
	int idTrooper = g_livingEntityManager.getIdLivingEntityInRange(m_idEntity, m_range);
	return idTrooper;
}

void Tower::render(Camera& camera, Light& light, SpriteBatch& spriteBatch)
{
	m_tower.render(camera, &light);
	m_bloodBar->render(spriteBatch, camera, m_instance->Position + MyVec3(-1, 5.0, 0), getHealthRatio());
}

FileMesh1::Instance* Tower::getInstance()
{
	return m_instance;
}