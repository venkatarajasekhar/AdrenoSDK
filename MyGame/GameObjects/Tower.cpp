
#include "Tower.h"
#include "Tower_Main.h"

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

#pragma region Constants

//=========================================================================================================
//
// Constants
//
//=========================================================================================================

static const int    TOWER_INITIAL_MAX_HEALTH = 1500;
static const int    TOWER_INITIAL_DAMAGE = 7;
static const float  TOWER_ATTACK_RANGE = 7;
static const MyVec2 TOWER_BLOOD_BAR_SCALE = MyVec2(1.0f, 0.6f);

static const int    MAIN_TOWER_INITIAL_MAX_HEALTH = 2000;
static const int    MAIN_TOWER_INITIAL_DAMAGE = 10;
static const float  MAIN_TOWER_ATTACK_RANGE = 10;
static const MyVec2 MAIN_TOWER_BLOOD_BAR_SCALE = MyVec2(1.5f, 0.6f);

static const MyVec3 MY_TOWER_SCALE = MyVec3(0.5f);
static const MyVec3 ENEMY_TOWER_SCALE = MyVec3(0.3f);

#pragma endregion

#pragma region Tower properties

//===================================================================================================================
//
// Tower properties
//
//===================================================================================================================

struct TowerProps
{
	int InitialMaxHealth;
	int InitialDamage;

	float AttackRange;

	MyVec2 BloodbarScale;
	MyVec3 BloodbarOffset;

	Material Material;
};

enum
{
	TOWER_WHITE_PAGODA,
	TOWER_HOUSE_WIND,
	TOWER_OUTPOST,
	TOWER_TOWER_OF_VICTORY,
	NUM_TOWERS,
};

static TowerProps g_TowerProps[NUM_TOWERS];

static void initTowerProps()
{
	// White pagoda
	g_TowerProps[TOWER_WHITE_PAGODA].InitialMaxHealth = MAIN_TOWER_INITIAL_MAX_HEALTH;
	g_TowerProps[TOWER_WHITE_PAGODA].InitialDamage = MAIN_TOWER_INITIAL_DAMAGE;

	g_TowerProps[TOWER_WHITE_PAGODA].AttackRange = MAIN_TOWER_ATTACK_RANGE;

	g_TowerProps[TOWER_WHITE_PAGODA].BloodbarScale = MAIN_TOWER_BLOOD_BAR_SCALE;
	g_TowerProps[TOWER_WHITE_PAGODA].BloodbarOffset = MyVec3(0, 8.5f, 0);

	g_TowerProps[TOWER_WHITE_PAGODA].Material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_TowerProps[TOWER_WHITE_PAGODA].Material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_TowerProps[TOWER_WHITE_PAGODA].Material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_TowerProps[TOWER_WHITE_PAGODA].Material.Shininess = 16.0f;

	// House wind
	g_TowerProps[TOWER_HOUSE_WIND].InitialMaxHealth = MAIN_TOWER_INITIAL_MAX_HEALTH;
	g_TowerProps[TOWER_HOUSE_WIND].InitialDamage = MAIN_TOWER_INITIAL_DAMAGE;

	g_TowerProps[TOWER_HOUSE_WIND].AttackRange = MAIN_TOWER_ATTACK_RANGE;

	g_TowerProps[TOWER_HOUSE_WIND].BloodbarScale = MAIN_TOWER_BLOOD_BAR_SCALE;
	g_TowerProps[TOWER_HOUSE_WIND].BloodbarOffset = MyVec3(0, 8, 0);

	g_TowerProps[TOWER_HOUSE_WIND].Material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_TowerProps[TOWER_HOUSE_WIND].Material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_TowerProps[TOWER_HOUSE_WIND].Material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_TowerProps[TOWER_HOUSE_WIND].Material.Shininess = 16.0f;

	// Outpost
	g_TowerProps[TOWER_OUTPOST].InitialMaxHealth = TOWER_INITIAL_MAX_HEALTH;
	g_TowerProps[TOWER_OUTPOST].InitialDamage = TOWER_INITIAL_DAMAGE;

	g_TowerProps[TOWER_OUTPOST].AttackRange = TOWER_ATTACK_RANGE;

	g_TowerProps[TOWER_OUTPOST].BloodbarScale = TOWER_BLOOD_BAR_SCALE;
	g_TowerProps[TOWER_OUTPOST].BloodbarOffset = MyVec3(0, 7, 0);

	g_TowerProps[TOWER_OUTPOST].Material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_TowerProps[TOWER_OUTPOST].Material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_TowerProps[TOWER_OUTPOST].Material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_TowerProps[TOWER_OUTPOST].Material.Shininess = 16.0f;

	// Tower of victory
	g_TowerProps[TOWER_TOWER_OF_VICTORY].InitialMaxHealth = TOWER_INITIAL_MAX_HEALTH;
	g_TowerProps[TOWER_TOWER_OF_VICTORY].InitialDamage = TOWER_INITIAL_DAMAGE;

	g_TowerProps[TOWER_TOWER_OF_VICTORY].AttackRange = TOWER_ATTACK_RANGE;

	g_TowerProps[TOWER_TOWER_OF_VICTORY].BloodbarScale = TOWER_BLOOD_BAR_SCALE;
	g_TowerProps[TOWER_TOWER_OF_VICTORY].BloodbarOffset = MyVec3(0, 7, 0);

	g_TowerProps[TOWER_TOWER_OF_VICTORY].Material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_TowerProps[TOWER_TOWER_OF_VICTORY].Material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_TowerProps[TOWER_TOWER_OF_VICTORY].Material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_TowerProps[TOWER_TOWER_OF_VICTORY].Material.Shininess = 16.0f;
}

#pragma endregion

#pragma region Tower in-game properties

//===================================================================================================================
//
// Tower in-game properties
//
//===================================================================================================================

struct TowerInGameProp
{
	MyVec3 Pos;
	MyVec3 Rot;
	MyVec3 Scale;
};

static TowerInGameProp g_TowerInGameProps[TowerPool::MAX_NUM_TOWER_IN_GAME];

static void initTowerInGameProps()
{
	// My main tower
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_MAIN_TOWER].Pos = MyVec3(-43.0f, 0, 0.39f);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_MAIN_TOWER].Rot = MyVec3(0);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_MAIN_TOWER].Scale = MyVec3(0.5f);

	// My tower 1
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_TOWER_1].Pos = MyVec3(-29.0f, 0, 2.0f);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_TOWER_1].Rot = MyVec3(0);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_TOWER_1].Scale = MY_TOWER_SCALE;

	// My tower 2
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_TOWER_2].Pos = MyVec3(-13.0f, 0, -3.5f);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_TOWER_2].Rot = MyVec3(0);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_TOWER_2].Scale = MY_TOWER_SCALE;

	// Enemy main tower
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_MAIN_TOWER].Pos = MyVec3(39.0f, 0, -1.5f);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_MAIN_TOWER].Rot = MyVec3(-90, 0, 0);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_MAIN_TOWER].Scale = MyVec3(0.002f);

	// Enemy tower 1
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_TOWER_1].Pos = MyVec3(15.0f, 0, 4.0f);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_TOWER_1].Rot = MyVec3(0);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_TOWER_1].Scale = ENEMY_TOWER_SCALE;

	// Enemy tower 2
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_TOWER_2].Pos = MyVec3(2.0f, 0, 4.5f);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_TOWER_2].Rot = MyVec3(0);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_TOWER_2].Scale = ENEMY_TOWER_SCALE;
}

#pragma endregion

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

void Tower::init(
	FileMesh1& mesh,
	BloodBar& bloodBar,
	std::vector<LivingEntity*>& lEnts,
	int iTower,
	int iTowerInGame,
	TEAM_TYPE team)
{
	TowerProps* towerProp = g_TowerProps + iTower;
	TowerInGameProp* towerInGameProp = g_TowerInGameProps + iTowerInGame;

	m_instance = Mesh::buildMeshInstance(towerInGameProp->Pos, towerInGameProp->Rot, towerInGameProp->Scale);
	mesh.addInstance(m_instance);

	setTeamType(team);
	setEntityType(ENTITY_TYPE_TOWER);

	LivingEntity::init(
		towerProp->InitialMaxHealth, 
		towerProp->InitialDamage, 
		bloodBar, 
		towerProp->BloodbarScale, 
		towerProp->BloodbarOffset, 
		lEnts, 
		towerProp->AttackRange);
}

void Tower::update(Timer& timer)
{
}

MyVec3 Tower::getPos()
{
	return m_instance->Position;
}

//=========================================================================================================
//
// TowerPool class
//
//=========================================================================================================

TowerPool::TowerPool()
{
	m_towers[TOWER_IN_GAME_MY_MAIN_TOWER] = new Tower_Main;
	m_towers[TOWER_IN_GAME_MY_TOWER_1] = new Tower;
	m_towers[TOWER_IN_GAME_MY_TOWER_2] = new Tower;

	m_towers[TOWER_IN_GAME_ENEMY_MAIN_TOWER] = new Tower_Main;
	m_towers[TOWER_IN_GAME_ENEMY_TOWER_1] = new Tower;
	m_towers[TOWER_IN_GAME_ENEMY_TOWER_2] = new Tower;
}

TowerPool::~TowerPool()
{
	for (int i = 0; i < MAX_NUM_TOWER_IN_GAME; i++)
	{
		SAFE_DELETE(m_towers[i]);
	}
}

void TowerPool::init(
	Shader& meshShader, 
	BloodBar& myBloodBar, 
	BloodBar& enemyBloodBar, 
	std::vector<LivingEntity*>& lEnts,
	IOnGameOverListener* gameOverListener)
{
	initTowerProps();
	initTowerInGameProps();

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
	m_fileMeshes[FILE_MESH_OUTPOST].init(
		m_mesh1Datas[MESH_1_DATA_OUTPOST], 
		m_meshTextures[TEXTURES_MESH_OUTPOST], 
		meshShader, 
		&g_TowerProps[TOWER_OUTPOST].Material);
	m_fileMeshes[FILE_MESH_WHITE_PAGODA].init(
		m_mesh1Datas[MESH_1_DATA_WHITE_PAGODA], 
		m_meshTextures[TEXTURES_MESH_WHITE_PAGODA], 
		meshShader, 
		&g_TowerProps[TOWER_WHITE_PAGODA].Material);
	m_fileMeshes[FILE_MESH_TOWER_OF_VICTORY].init(
		m_mesh1Datas[MESH_1_DATA_TOWER_OF_VICTORY], 
		m_meshTextures[TEXTURES_MESH_TOWER_OF_VICTORY], 
		meshShader, 
		&g_TowerProps[TOWER_TOWER_OF_VICTORY].Material);
	m_fileMeshes[FILE_MESH_HOUSE_WIND].init(
		m_mesh1Datas[MESH_1_DATA_HOUSE_WIND], 
		m_meshTextures[TEXTURES_MESH_HOUSE_WIND], 
		meshShader, 
		&g_TowerProps[TOWER_HOUSE_WIND].Material);

	// Towers
	m_towers[TOWER_IN_GAME_MY_MAIN_TOWER]->init(m_fileMeshes[FILE_MESH_WHITE_PAGODA], myBloodBar, lEnts, TOWER_WHITE_PAGODA, TOWER_IN_GAME_MY_MAIN_TOWER, TEAM_TYPE_MY_TEAM);
	m_towers[TOWER_IN_GAME_MY_TOWER_1]->init(m_fileMeshes[FILE_MESH_OUTPOST], myBloodBar, lEnts, TOWER_OUTPOST, TOWER_IN_GAME_MY_TOWER_1, TEAM_TYPE_MY_TEAM);
	m_towers[TOWER_IN_GAME_MY_TOWER_2]->init(m_fileMeshes[FILE_MESH_OUTPOST], myBloodBar, lEnts, TOWER_OUTPOST, TOWER_IN_GAME_MY_TOWER_2, TEAM_TYPE_MY_TEAM);

	m_towers[TOWER_IN_GAME_ENEMY_MAIN_TOWER]->init(m_fileMeshes[FILE_MESH_HOUSE_WIND], enemyBloodBar, lEnts, TOWER_HOUSE_WIND, TOWER_IN_GAME_ENEMY_MAIN_TOWER, TEAM_TYPE_ENEMY);
	m_towers[TOWER_IN_GAME_ENEMY_TOWER_1]->init(m_fileMeshes[FILE_MESH_TOWER_OF_VICTORY], enemyBloodBar, lEnts, TOWER_TOWER_OF_VICTORY, TOWER_IN_GAME_ENEMY_TOWER_1, TEAM_TYPE_ENEMY);
	m_towers[TOWER_IN_GAME_ENEMY_TOWER_2]->init(m_fileMeshes[FILE_MESH_TOWER_OF_VICTORY], enemyBloodBar, lEnts, TOWER_TOWER_OF_VICTORY, TOWER_IN_GAME_ENEMY_TOWER_2, TEAM_TYPE_ENEMY);

	((Tower_Main*)m_towers[TOWER_IN_GAME_MY_MAIN_TOWER])->addGameOverListener(gameOverListener);
	((Tower_Main*)m_towers[TOWER_IN_GAME_ENEMY_MAIN_TOWER])->addGameOverListener(gameOverListener);

	// Fill into list of living entities
	for (size_t i = 0; i < MAX_NUM_TOWER_IN_GAME; i++)
	{
		lEnts.push_back(m_towers[i]);
	}
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