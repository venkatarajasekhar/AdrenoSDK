
#include "Hero.h"
#include "Hero_AI.h"
#include "Hero_Controlled.h"

#pragma region Hero properties

//===================================================================================================================
//
// Hero properties
//
//===================================================================================================================

struct HeroProps
{
	int InitialMaxHealth;
	int InitialDamage;

	float AttackRange;

	float MovingSpeed;
	float MovingRotYOffset;
	float MovingTurnSpeed;

	MyVec3 BloodbarOffset;

	Material Material;

	MyVec3 InitialPos;
	MyVec3 InitialRot;
	MyVec3 InitialScale;
};

enum
{
	HERO_BEAST_SEWON,
	HERO_FIGHTER_DAN_MEI,
	NUM_HEROES,
};

static HeroProps g_HeroProps[NUM_HEROES];

static void initHeroProps()
{
	// Beast sewon
	g_HeroProps[HERO_BEAST_SEWON].InitialMaxHealth = 1000;
	g_HeroProps[HERO_BEAST_SEWON].InitialDamage = 10;

	g_HeroProps[HERO_BEAST_SEWON].AttackRange = 5;

	g_HeroProps[HERO_BEAST_SEWON].MovingSpeed = 5;
	g_HeroProps[HERO_BEAST_SEWON].MovingRotYOffset = 0;
	g_HeroProps[HERO_BEAST_SEWON].MovingTurnSpeed = 500;

	g_HeroProps[HERO_BEAST_SEWON].BloodbarOffset = MyVec3(0, 6, 0);

	g_HeroProps[HERO_BEAST_SEWON].Material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_HeroProps[HERO_BEAST_SEWON].Material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_HeroProps[HERO_BEAST_SEWON].Material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_HeroProps[HERO_BEAST_SEWON].Material.Shininess = 16.0f;

	g_HeroProps[HERO_BEAST_SEWON].InitialPos = MyVec3(17.4f, 0, -1.0f);
	g_HeroProps[HERO_BEAST_SEWON].InitialRot = MyVec3(0, -90, 0);
	g_HeroProps[HERO_BEAST_SEWON].InitialScale = MyVec3(0.01f);

	// Fighter dan mei
	g_HeroProps[HERO_FIGHTER_DAN_MEI].InitialMaxHealth = 1000;
	g_HeroProps[HERO_FIGHTER_DAN_MEI].InitialDamage = 10;

	g_HeroProps[HERO_FIGHTER_DAN_MEI].AttackRange = 10;

	g_HeroProps[HERO_FIGHTER_DAN_MEI].MovingSpeed = 5;
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MovingRotYOffset = 0;
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MovingTurnSpeed = 500;

	g_HeroProps[HERO_FIGHTER_DAN_MEI].BloodbarOffset = MyVec3(-1, 6, 0);

	g_HeroProps[HERO_FIGHTER_DAN_MEI].Material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].Material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].Material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].Material.Shininess = 16.0f;

	g_HeroProps[HERO_FIGHTER_DAN_MEI].InitialPos = MyVec3(-25.0f, 0, -8.0f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].InitialRot = MyVec3(0, 90, 0);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].InitialScale = MyVec3(0.015f);
}

#pragma endregion

#pragma region Constants

//===================================================================================================================
//
// Constants
//
//===================================================================================================================

static const MyVec2 HERO_BLOOD_BAR_SCALE = MyVec2(1.0f, 1.0f);

#pragma endregion

//===================================================================================================================
//
// Hero class
//
//===================================================================================================================

Hero::Hero()
	: m_instance(nullptr),
	m_stateMachine(nullptr)
{
}

Hero::~Hero()
{
	SAFE_DELETE(m_stateMachine);
}

// Core functions

void Hero::init(
	SkinnedMesh1& mesh,
	BloodBar& bloodBar,
	std::vector<LivingEntity*>& lEnts,
	int iHero,
	TEAM_TYPE team)
{
	HeroProps* heroProps = g_HeroProps + iHero;

	m_instance = SkinnedMesh1::buildSkinnedMeshInstance(heroProps->InitialPos, heroProps->InitialRot, heroProps->InitialScale, "idle");
	mesh.addInstance(m_instance);

	// Moving elements
	m_movingEnt.init(
		m_instance->Position, 
		m_instance->Position, 
		m_instance->Rotation,
		heroProps->MovingRotYOffset, 
		heroProps->MovingSpeed, 
		heroProps->MovingTurnSpeed);

	// States manager
	m_stateMachine = new StateMachine<Hero>(this);
	m_stateMachine->SetCurrentState(HeroState_Idle::instance());

	setTeamType(team);
	setEntityType(ENTITY_TYPE_HERO);

	LivingEntity::init(
		heroProps->InitialMaxHealth, 
		heroProps->InitialDamage, 
		bloodBar, 
		HERO_BLOOD_BAR_SCALE, 
		heroProps->BloodbarOffset, 
		lEnts, 
		heroProps->AttackRange);
}

void Hero::update(Timer& timer)
{
	// Moving elements
	m_movingEnt.update(timer);

	m_instance->Position = m_movingEnt.getPos();
	m_instance->Rotation = m_movingEnt.getRot();

	// States manager
	m_stateMachine->Update();
}

MyVec3 Hero::getPos()
{
	return m_movingEnt.getPos();
}

//===================================================================================================================
//
// HeroPool class
//
//===================================================================================================================

HeroPool::HeroPool()
{
	m_heroes[HERO_IN_GAME_MY_HERO_1] = new Hero_Controlled;
	m_heroes[HERO_IN_GAME_ENEMY_HERO_1] = new Hero_AI;
}

HeroPool::~HeroPool()
{
	for (int i = 0; i < MAX_NUM_HEROES_IN_GAME; i++)
	{
		SAFE_DELETE(m_heroes[i]);
	}
}

void HeroPool::init(Shader& skinnedShader, BloodBar& myBloodBar, BloodBar& enemyBloodBar, std::vector<LivingEntity*>& lEnts, OnPressListenee& map)
{
	initHeroProps();

	// Assets mesh data
	m_mesh1Datas[MESH_1_DATA_BEAST_SEWON].init(resolveAssetsPath("Meshes/Heroes/Beast/sewon/Sewon.model"));
	m_mesh1Datas[MESH_1_DATA_FIGHTER_DAN_MEI].init(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/DanMei_A_Type.model"));

	// Assets anim data
	{
		const int numAnimFiles = 5;
		SkinnedMesh1::AnimFile animFiles[numAnimFiles] =
		{
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/idle.anim"), "idle"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/run.anim"), "run"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/cast.anim"), "attack_1"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/la.anim"), "attack_2"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/la2.anim"), "attack_3"),
		};
		m_anim1Datas[ANIM_1_DATA_BEAST_SEWON].init(animFiles, numAnimFiles);
	}

	{
		const int numAnimFiles = 5;
		SkinnedMesh1::AnimFile animFiles[numAnimFiles] =
		{
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/idle.anim"), "idle"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/fwd.anim"), "run"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/la2.anim"), "attack_1"), // Normal attack
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/la.anim"), "attack_2"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/llla.anim"), "attack_3"),
		};
		m_anim1Datas[ANIM_1_DATA_FIGHTER_DAN_MEI].init(animFiles, numAnimFiles);
	}

	// Assets mesh texture
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/Sewon.pak").c_str());
		m_meshTextures[TEXTURES_MESH_BEAST_SEWON].init(m_mesh1Datas[MESH_1_DATA_BEAST_SEWON], resource);
	}
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/DanMei_A_Type.pak").c_str());
		m_meshTextures[TEXTURES_MESH_FIGHTER_DAN_MEI].init(m_mesh1Datas[MESH_1_DATA_FIGHTER_DAN_MEI], resource);
	}

	// Skinned meshes
	m_skinnedMeshes[SKINNED_MESH_BEAST_SEWON].init(
		m_mesh1Datas[MESH_1_DATA_BEAST_SEWON], 
		m_anim1Datas[ANIM_1_DATA_BEAST_SEWON], 
		m_meshTextures[TEXTURES_MESH_BEAST_SEWON], 
		skinnedShader, 
		&g_HeroProps[HERO_BEAST_SEWON].Material);
	m_skinnedMeshes[SKINNED_MESH_FIGHTER_DAN_MEI].init(
		m_mesh1Datas[MESH_1_DATA_FIGHTER_DAN_MEI], 
		m_anim1Datas[ANIM_1_DATA_FIGHTER_DAN_MEI], 
		m_meshTextures[TEXTURES_MESH_FIGHTER_DAN_MEI], 
		skinnedShader, 
		&g_HeroProps[HERO_FIGHTER_DAN_MEI].Material);

	// Adjusting unaligned action
	m_skinnedMeshes[SKINNED_MESH_FIGHTER_DAN_MEI].translateAction("attack_1", MyVec3(0, 0, -200));
	
	// Heroes
	m_heroes[HERO_IN_GAME_MY_HERO_1]->init(m_skinnedMeshes[SKINNED_MESH_FIGHTER_DAN_MEI], myBloodBar, lEnts, HERO_FIGHTER_DAN_MEI, TEAM_TYPE_MY_TEAM);
	m_heroes[HERO_IN_GAME_ENEMY_HERO_1]->init(m_skinnedMeshes[SKINNED_MESH_BEAST_SEWON], enemyBloodBar, lEnts, HERO_BEAST_SEWON, TEAM_TYPE_ENEMY);

	map.addPressListener((Hero_Controlled*)m_heroes[0]);

	// Fill into list of living entities
	for (size_t i = 0; i < MAX_NUM_HEROES_IN_GAME; i++)
	{
		lEnts.push_back(m_heroes[i]);
	}
}

void HeroPool::update(Timer& timer)
{
	for (int i = 0; i < NUM_SKINNED_MESHES; i++)
	{
		m_skinnedMeshes[i].update(timer);
	}
}

void HeroPool::render(Camera& camera, Light& light)
{
	for (int i = 0; i < NUM_SKINNED_MESHES; i++)
	{
		m_skinnedMeshes[i].render(camera, &light);
	}
}

#pragma region Hero states

//===================================================================================================================
//
// Hero state idle
//
//===================================================================================================================

void HeroState_Idle::Enter(Hero* hero)
{
	hero->m_instance->setAction("idle");
}

void HeroState_Idle::Execute(Hero* hero)
{
	if (hero->m_movingEnt.isMoving())
	{
		hero->m_stateMachine->ChangeState(HeroState_Walk::instance());
	}
	else
	{
		for (auto i = hero->m_lEnts->begin(); i != hero->m_lEnts->end(); ++i)
		{
			if ((hero != (*i)) && 
				(hero->getTeamType() != (*i)->getTeamType()) &&
				(distance_optimized(hero->getPos(), (*i)->getPos()) <= hero->m_atkRange))
			{
				hero->m_atkTarget = (*i);
				hero->m_stateMachine->ChangeState(HeroState_Attack::instance());
				break;
			}
		}
	}
}

void HeroState_Idle::Exit(Hero* hero)
{

}

//===================================================================================================================
//
// Hero state walk
//
//===================================================================================================================

void HeroState_Walk::Enter(Hero* hero)
{
	hero->m_instance->setAction("run");
}

void HeroState_Walk::Execute(Hero* hero)
{
	if (!hero->m_movingEnt.isMoving())
	{
		hero->m_stateMachine->ChangeState(HeroState_Idle::instance());
	}
}

void HeroState_Walk::Exit(Hero* hero)
{

}

//===================================================================================================================
//
// Hero state attack
//
//===================================================================================================================

void HeroState_Attack::Enter(Hero* hero)
{
	hero->m_instance->setAction("attack_1");
}

void HeroState_Attack::Execute(Hero* hero)
{
	if (hero->m_movingEnt.isMoving())
	{
		hero->m_stateMachine->ChangeState(HeroState_Walk::instance());
	}
	else
	{
		if (hero->m_atkTarget != nullptr)
		{
			if (distance_optimized(hero->getPos(), hero->m_atkTarget->getPos()) > hero->m_atkRange)
			{
				hero->m_stateMachine->ChangeState(HeroState_Idle::instance());
			}
			else
			{
				hero->m_atkTarget->accHealth(-hero->m_damage);
			}
		}
	}
}

void HeroState_Attack::Exit(Hero* hero)
{

}

#pragma endregion