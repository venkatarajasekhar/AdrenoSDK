
#include "Tower.h"
#include "Tower_Main.h"

#pragma region Constants

//=========================================================================================================
//
// Constants
//
//=========================================================================================================

static const int    TOWER_INITIAL_MAX_HEALTH = 1000;
static const int    TOWER_INITIAL_DAMAGE = 25;
static const float  TOWER_ATTACK_RANGE = 10;
static const float  TOWER_ATTACK_TIME_PERIOD = 4;
static const MyVec2 TOWER_BLOOD_BAR_SCALE = MyVec2(3.0f, 1.0f);

static const int    MAIN_TOWER_INITIAL_MAX_HEALTH = 2000;
static const int    MAIN_TOWER_INITIAL_DAMAGE = 40;
static const float  MAIN_TOWER_ATTACK_RANGE = 10;
static const MyVec2 MAIN_TOWER_BLOOD_BAR_SCALE = MyVec2(4.0f, 1.0f);

static const MyVec3 MY_TOWER_SCALE = MyVec3(0.5f);
static const MyVec3 ENEMY_TOWER_SCALE = MyVec3(0.3f);

#pragma endregion

#pragma region Tower properties

//===================================================================================================================
//
// Tower properties
//
//===================================================================================================================

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
	g_TowerProps[TOWER_WHITE_PAGODA].BloodbarOffset = MyVec3(-2.0f, 9.5f, 0);

	g_TowerProps[TOWER_WHITE_PAGODA].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_TowerProps[TOWER_WHITE_PAGODA].MeshMaterial.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_TowerProps[TOWER_WHITE_PAGODA].MeshMaterial.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_TowerProps[TOWER_WHITE_PAGODA].MeshMaterial.Shininess = 16.0f;

	// House wind
	g_TowerProps[TOWER_HOUSE_WIND].InitialMaxHealth = MAIN_TOWER_INITIAL_MAX_HEALTH;
	g_TowerProps[TOWER_HOUSE_WIND].InitialDamage = MAIN_TOWER_INITIAL_DAMAGE;

	g_TowerProps[TOWER_HOUSE_WIND].AttackRange = MAIN_TOWER_ATTACK_RANGE;

	g_TowerProps[TOWER_HOUSE_WIND].BloodbarScale = MAIN_TOWER_BLOOD_BAR_SCALE;
	g_TowerProps[TOWER_HOUSE_WIND].BloodbarOffset = MyVec3(-2.0f, 9, 0);

	g_TowerProps[TOWER_HOUSE_WIND].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_TowerProps[TOWER_HOUSE_WIND].MeshMaterial.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_TowerProps[TOWER_HOUSE_WIND].MeshMaterial.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_TowerProps[TOWER_HOUSE_WIND].MeshMaterial.Shininess = 16.0f;

	// Outpost
	g_TowerProps[TOWER_OUTPOST].InitialMaxHealth = TOWER_INITIAL_MAX_HEALTH;
	g_TowerProps[TOWER_OUTPOST].InitialDamage = TOWER_INITIAL_DAMAGE;

	g_TowerProps[TOWER_OUTPOST].AttackRange = TOWER_ATTACK_RANGE;

	g_TowerProps[TOWER_OUTPOST].BloodbarScale = TOWER_BLOOD_BAR_SCALE;
	g_TowerProps[TOWER_OUTPOST].BloodbarOffset = MyVec3(-2.0f, 8, 0);

	g_TowerProps[TOWER_OUTPOST].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_TowerProps[TOWER_OUTPOST].MeshMaterial.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_TowerProps[TOWER_OUTPOST].MeshMaterial.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_TowerProps[TOWER_OUTPOST].MeshMaterial.Shininess = 16.0f;

	// Tower of victory
	g_TowerProps[TOWER_TOWER_OF_VICTORY].InitialMaxHealth = TOWER_INITIAL_MAX_HEALTH;
	g_TowerProps[TOWER_TOWER_OF_VICTORY].InitialDamage = TOWER_INITIAL_DAMAGE;

	g_TowerProps[TOWER_TOWER_OF_VICTORY].AttackRange = TOWER_ATTACK_RANGE;

	g_TowerProps[TOWER_TOWER_OF_VICTORY].BloodbarScale = TOWER_BLOOD_BAR_SCALE;
	g_TowerProps[TOWER_TOWER_OF_VICTORY].BloodbarOffset = MyVec3(-2.0f, 8, 0);

	g_TowerProps[TOWER_TOWER_OF_VICTORY].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_TowerProps[TOWER_TOWER_OF_VICTORY].MeshMaterial.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_TowerProps[TOWER_TOWER_OF_VICTORY].MeshMaterial.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_TowerProps[TOWER_TOWER_OF_VICTORY].MeshMaterial.Shininess = 16.0f;
}

#pragma endregion

#pragma region Tower in-game properties

//===================================================================================================================
//
// Tower in-game properties
//
//===================================================================================================================

static TowerInGameProp g_TowerInGameProps[TowerPool::MAX_NUM_TOWER_IN_GAME];

static void initTowerInGameProps()
{
	// My main tower
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_MAIN_TOWER].Pos = MyVec3(-43.0f, 0, 0.39f);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_MAIN_TOWER].Rot = MyVec3(0);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_MAIN_TOWER].Scale = MyVec3(0.5f);

	// My tower 1
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_TOWER_1].Pos = MyVec3(-30.0f, 0, 2.0f);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_TOWER_1].Rot = MyVec3(0);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_TOWER_1].Scale = MY_TOWER_SCALE;

	// My tower 2
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_TOWER_2].Pos = MyVec3(-10.0f, 0, -3.5f);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_TOWER_2].Rot = MyVec3(0);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_MY_TOWER_2].Scale = MY_TOWER_SCALE;

	// Enemy main tower
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_MAIN_TOWER].Pos = MyVec3(39.0f, 0, -1.5f);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_MAIN_TOWER].Rot = MyVec3(-90, 0, 0);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_MAIN_TOWER].Scale = MyVec3(0.002f);

	// Enemy tower 1
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_TOWER_1].Pos = MyVec3(30.0f, 0, -4.0f);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_TOWER_1].Rot = MyVec3(0);
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_TOWER_1].Scale = ENEMY_TOWER_SCALE;

	// Enemy tower 2
	g_TowerInGameProps[TowerPool::TOWER_IN_GAME_ENEMY_TOWER_2].Pos = MyVec3(10.0f, 0, 4.5f);
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
	: m_instance(nullptr),
	m_timeElapsed(TOWER_ATTACK_TIME_PERIOD)
{
	m_stateMachine = new StateMachine<Tower>(this);
}

Tower::~Tower()
{
	SAFE_DELETE(m_stateMachine);
}

void Tower::init(
	FileMesh1& mesh,
	BloodBar& bloodBar,
	Quad3D& selectedDecal,
	Billboard& projtBillboard,
	ProjectilePool& projectilePool,
	std::vector<LivingEntity*>& lEnts,
	Audio lAudios[],
	TowerProps& towerProp,
	TowerInGameProp& towerInGameProp,
	TEAM_TYPE team)
{
	m_projtBillboard = &projtBillboard;
	m_projectilePool = &projectilePool;

	m_instance = Mesh::buildMeshInstance(towerInGameProp.Pos, towerInGameProp.Rot, towerInGameProp.Scale);
	mesh.addInstance(m_instance);

	setTeamType(team);
	setEntityType(ENTITY_TYPE_TOWER);

	m_inUse = true;

	// States manager
	m_stateMachine->SetCurrentState(TowerState_Idle::instance());

	LivingEntity::init(
		towerProp.InitialMaxHealth, 
		towerProp.InitialDamage, 
		bloodBar, 
		towerProp.BloodbarScale, 
		towerProp.BloodbarOffset, 
		lEnts, 
		towerProp.AttackRange,
		&selectedDecal);

	m_audios = lAudios;
}

void Tower::update(Timer& timer)
{
	m_timeElapsed += timer.getElapsedTime();

	// States manager
	m_stateMachine->Update();
}

MyVec3 Tower::getPos()
{
	return m_instance->Position;
}

void Tower::dead()
{
	m_audios[AUDIO_TOWER_DEATH].play();
	m_instance->Visible = false;
	LivingEntity::dead();
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
	Quad3D& selectedDecal,
	Billboard& projtBillboard,
	ProjectilePool& projectilePool,
	std::vector<LivingEntity*>& lEnts,
	Audio lAudios[],
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
		&g_TowerProps[TOWER_OUTPOST].MeshMaterial);
	m_fileMeshes[FILE_MESH_WHITE_PAGODA].init(
		m_mesh1Datas[MESH_1_DATA_WHITE_PAGODA], 
		m_meshTextures[TEXTURES_MESH_WHITE_PAGODA], 
		meshShader, 
		&g_TowerProps[TOWER_WHITE_PAGODA].MeshMaterial);
	m_fileMeshes[FILE_MESH_TOWER_OF_VICTORY].init(
		m_mesh1Datas[MESH_1_DATA_TOWER_OF_VICTORY], 
		m_meshTextures[TEXTURES_MESH_TOWER_OF_VICTORY], 
		meshShader, 
		&g_TowerProps[TOWER_TOWER_OF_VICTORY].MeshMaterial);
	m_fileMeshes[FILE_MESH_HOUSE_WIND].init(
		m_mesh1Datas[MESH_1_DATA_HOUSE_WIND], 
		m_meshTextures[TEXTURES_MESH_HOUSE_WIND], 
		meshShader, 
		&g_TowerProps[TOWER_HOUSE_WIND].MeshMaterial);

	// Towers
	m_towers[TOWER_IN_GAME_MY_MAIN_TOWER]->init(
		m_fileMeshes[FILE_MESH_WHITE_PAGODA], 
		myBloodBar, 
		selectedDecal,
		projtBillboard,
		projectilePool,
		lEnts, 
		lAudios,
		g_TowerProps[TOWER_WHITE_PAGODA], 
		g_TowerInGameProps[TOWER_IN_GAME_MY_MAIN_TOWER], 
		TEAM_TYPE_MY_TEAM);
	m_towers[TOWER_IN_GAME_MY_TOWER_1]->init(
		m_fileMeshes[FILE_MESH_OUTPOST], 
		myBloodBar, 
		selectedDecal,
		projtBillboard,
		projectilePool,
		lEnts, 
		lAudios,
		g_TowerProps[TOWER_OUTPOST], 
		g_TowerInGameProps[TOWER_IN_GAME_MY_TOWER_1], 
		TEAM_TYPE_MY_TEAM);
	m_towers[TOWER_IN_GAME_MY_TOWER_2]->init(
		m_fileMeshes[FILE_MESH_OUTPOST], 
		myBloodBar, 
		selectedDecal,
		projtBillboard,
		projectilePool,
		lEnts, 
		lAudios,
		g_TowerProps[TOWER_OUTPOST], 
		g_TowerInGameProps[TOWER_IN_GAME_MY_TOWER_2], 
		TEAM_TYPE_MY_TEAM);

	m_towers[TOWER_IN_GAME_ENEMY_MAIN_TOWER]->init(
		m_fileMeshes[FILE_MESH_HOUSE_WIND], 
		enemyBloodBar,
		selectedDecal,
		projtBillboard,
		projectilePool,
		lEnts, 
		lAudios,
		g_TowerProps[TOWER_HOUSE_WIND], 
		g_TowerInGameProps[TOWER_IN_GAME_ENEMY_MAIN_TOWER], 
		TEAM_TYPE_ENEMY);
	m_towers[TOWER_IN_GAME_ENEMY_TOWER_1]->init(
		m_fileMeshes[FILE_MESH_TOWER_OF_VICTORY], 
		enemyBloodBar, 
		selectedDecal,
		projtBillboard,
		projectilePool,
		lEnts, 
		lAudios,
		g_TowerProps[TOWER_TOWER_OF_VICTORY], 
		g_TowerInGameProps[TOWER_IN_GAME_ENEMY_TOWER_1], 
		TEAM_TYPE_ENEMY);
	m_towers[TOWER_IN_GAME_ENEMY_TOWER_2]->init(
		m_fileMeshes[FILE_MESH_TOWER_OF_VICTORY], 
		enemyBloodBar, 
		selectedDecal,
		projtBillboard,
		projectilePool,
		lEnts, 
		lAudios,
		g_TowerProps[TOWER_TOWER_OF_VICTORY], 
		g_TowerInGameProps[TOWER_IN_GAME_ENEMY_TOWER_2], 
		TEAM_TYPE_ENEMY);

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

#pragma region Tower state

//=========================================================================================================
//
// Tower state idle
//
//=========================================================================================================

void TowerState_Idle::Enter(Tower* tower)
{
}

void TowerState_Idle::Execute(Tower* tower)
{
	for (auto i = tower->m_lEnts->begin(); i != tower->m_lEnts->end(); ++i)
	{
		if ((tower != (*i)) &&
			((*i)->inUse()) &&
			(tower->getTeamType() != (*i)->getTeamType()) &&
			(distance_optimized(tower->getPos(), (*i)->getPos()) <= tower->m_atkRange))
		{
			tower->m_atkTarget = (*i);
			tower->m_stateMachine->ChangeState(TowerState_Attack::instance());
			break;
		}
	}
}

void TowerState_Idle::Exit(Tower* tower)
{
}

//=========================================================================================================
//
// Tower state attack
//
//=========================================================================================================

void TowerState_Attack::Enter(Tower* tower)
{
}

void TowerState_Attack::Execute(Tower* tower)
{
	if (tower->m_atkTarget != nullptr)
	{
		if (
			(!tower->m_atkTarget->inUse()) ||
			(distance_optimized(tower->getPos(), tower->m_atkTarget->getPos()) > tower->m_atkRange)
			)
		{
			tower->m_stateMachine->ChangeState(TowerState_Idle::instance());
		}
		else
		{
			if (tower->m_timeElapsed > TOWER_ATTACK_TIME_PERIOD)
			{
				if (tower->getTeamType() == TEAM_TYPE_MY_TEAM) tower->m_audios[tower->AUDIO_MYTOWER_ATTACK].play();
				if (tower->getTeamType() == TEAM_TYPE_ENEMY) tower->m_audios[tower->AUDIO_ENEMYTOWER_ATTACK].play();
				
				tower->m_projectilePool->spawnProjectile(
					*tower->m_projtBillboard,
					tower,
					tower->m_atkTarget,
					MyVec3(0, 0, 0));

				tower->m_timeElapsed = 0;
			}
		}
	}
}

void TowerState_Attack::Exit(Tower* tower)
{
}

#pragma endregion