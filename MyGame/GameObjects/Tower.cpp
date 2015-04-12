
#include "Tower.h"

/*
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

		//m_tower.init(model, textures.Textures, shader, &material);

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
/**/

//----------------------------------------------------------------------------------------------------------------------------------------

//=========================================================================================================
//
// Tower class
//
//=========================================================================================================

Tower::Tower()
	: m_instance(nullptr)
{
}

Tower::~Tower()
{
}

void Tower::init(FileMesh1& mesh, const MyVec3& pos, const MyVec3& rot, const MyVec3& scale)
{
	m_instance = Mesh::buildMeshInstance(pos, rot, scale);
	mesh.addInstance(m_instance);
}

//=========================================================================================================
//
// TowerPool class
//
//=========================================================================================================

TowerPool::TowerPool()
{
}

TowerPool::~TowerPool()
{
}

void TowerPool::init(Shader& meshShader)
{
	// Assets mesh data
	m_mesh1Datas[MESH_1_DATA_HOUSE_WIND].init(resolveAssetsPath("Meshes/Towers/house_wind/wrhousewind02.model"));
	m_mesh1Datas[MESH_1_DATA_OUTPOST].init(resolveAssetsPath("Meshes/Towers/outpost/outpost.model"));
	m_mesh1Datas[MESH_1_DATA_TOWER_OF_VICTORY].init(resolveAssetsPath("Meshes/Towers/tower_of_victory/tower.model"));
	m_mesh1Datas[MESH_1_DATA_WHITE_PAGODA].init(resolveAssetsPath("Meshes/Towers/white_pagoda/white_pagoda.model"));

	// Assets mesh texture
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Meshes/Towers/house_wind/wrhousewind02.pak").c_str());
		m_meshTextures[TEXTURES_MESH_HOUSE_WIND].init(m_mesh1Datas[MESH_1_DATA_HOUSE_WIND], resource);
	}
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Meshes/Towers/outpost/outpost.pak").c_str());
		m_meshTextures[TEXTURES_MESH_OUTPOST].init(m_mesh1Datas[MESH_1_DATA_OUTPOST], resource);
	}
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Meshes/Towers/tower_of_victory/tower.pak").c_str());
		m_meshTextures[TEXTURES_MESH_TOWER_OF_VICTORY].init(m_mesh1Datas[MESH_1_DATA_TOWER_OF_VICTORY], resource);
	}
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Meshes/Towers/white_pagoda/white_pagoda.pak").c_str());
		m_meshTextures[TEXTURES_MESH_WHITE_PAGODA].init(m_mesh1Datas[MESH_1_DATA_WHITE_PAGODA], resource);
	}
	
	// File meshes
	Material material;

	material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	material.Shininess = 16.0f;

	m_fileMeshes[FILE_MESH_MY_TOWER].init(m_mesh1Datas[MESH_1_DATA_OUTPOST], m_meshTextures[TEXTURES_MESH_OUTPOST], meshShader, &material);
	m_fileMeshes[FILE_MESH_MY_MAIN_TOWER].init(m_mesh1Datas[MESH_1_DATA_WHITE_PAGODA], m_meshTextures[TEXTURES_MESH_WHITE_PAGODA], meshShader, &material);
	m_fileMeshes[FILE_MESH_ENEMY_TOWER].init(m_mesh1Datas[MESH_1_DATA_TOWER_OF_VICTORY], m_meshTextures[TEXTURES_MESH_TOWER_OF_VICTORY], meshShader, &material);
	m_fileMeshes[FILE_MESH_ENEMY_MAIN_TOWER].init(m_mesh1Datas[MESH_1_DATA_HOUSE_WIND], m_meshTextures[TEXTURES_MESH_HOUSE_WIND], meshShader, &material);

	// Towers
	m_towers[0].init(m_fileMeshes[FILE_MESH_MY_MAIN_TOWER], MyVec3(-43.0f, 0, 0.39f), MyVec3(0), MyVec3(0.5f));
	m_towers[1].init(m_fileMeshes[FILE_MESH_MY_TOWER], MyVec3(-29.0f, 0, 2.0f), MyVec3(0, -90, 0), MyVec3(0.5f));
	m_towers[2].init(m_fileMeshes[FILE_MESH_MY_TOWER], MyVec3(-13.0f, 0, -3.5f), MyVec3(0), MyVec3(0.5f));

	m_towers[3].init(m_fileMeshes[FILE_MESH_ENEMY_MAIN_TOWER], MyVec3(39.0f, 0, -1.5f), MyVec3(-90, 0 ,0), MyVec3(0.002f));
	m_towers[4].init(m_fileMeshes[FILE_MESH_ENEMY_TOWER], MyVec3(15.0f, 0, 4.0f), MyVec3(0, 45, 0), MyVec3(0.3f));
	m_towers[5].init(m_fileMeshes[FILE_MESH_ENEMY_TOWER], MyVec3(2.0f, 0, 4.5f), MyVec3(0), MyVec3(0.3f));
}

void TowerPool::update(Timer& timer)
{
	for (int i = 0; i < NUM_FILE_MESHES; i++)
	{
		m_fileMeshes[i].update(timer);
	}
}

void TowerPool::render(Camera& camera, Light& light)
{
	for (int i = 0; i < NUM_FILE_MESHES; i++)
	{
		m_fileMeshes[i].render(camera, &light);
	}
}