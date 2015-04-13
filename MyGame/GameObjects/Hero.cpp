
#include "Hero.h"
#include "Hero_AI.h"
#include "Hero_Controlled.h"

//=========================================================================================================
//
// Constants
//
//=========================================================================================================

static const int    HERO_INITIAL_MAX_HEALTH = 1000;
static const int    HERO_INITIAL_DAMAGE = 5;
static const MyVec2 HERO_BLOOD_BAR_SCALE = MyVec2(1.0f, 1.0f);

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

void Hero::init(SkinnedMesh1& mesh, const MyVec3& pos, const MyVec3& rot, const MyVec3& scale,
	BloodBar& bloodBar, const MyVec3& bloodBarOffset)
{
	m_instance = SkinnedMesh1::buildSkinnedMeshInstance(pos, rot, scale, "idle");
	mesh.addInstance(m_instance);

	// Moving elements
	m_movingEnt.init(m_instance->Position, m_instance->Position, m_instance->Rotation,
		0, 3.0f, 180.0f);

	// States manager
	m_stateMachine = new StateMachine<Hero>(this);
	m_stateMachine->SetCurrentState(HeroState_Idle::instance());

	LivingEntity::init(HERO_INITIAL_MAX_HEALTH, HERO_INITIAL_DAMAGE, bloodBar, HERO_BLOOD_BAR_SCALE, bloodBarOffset);
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

void Hero::render(Camera& camera, Light& light)
{
}

StateMachine<Hero>* Hero::getFSM()const
{
	return m_stateMachine;
}

MyVec3 Hero::getPos()
{
	return m_movingEnt.getPos();
}

// Controlling hero
void Hero::changeAnimAction(const MyString& action)
{
	m_instance->CurrentAction = action;
}

bool Hero::isMoving()
{
	return m_movingEnt.isMoving();
}

//===================================================================================================================
//
// HeroPool class
//
//===================================================================================================================

HeroPool::HeroPool()
{
	m_heroes[0] = new Hero_Controlled;
	m_heroes[1] = new Hero_AI;
}

HeroPool::~HeroPool()
{
	for (int i = 0; i < MAX_NUM_HEROES; i++)
	{
		SAFE_DELETE(m_heroes[i]);
	}
}

void HeroPool::init(Shader& skinnedShader, BloodBar& myBloodBar, BloodBar& enemyBloodBar, std::vector<LivingEntity*>& lEnts)
{
	// Assets mesh data
	m_mesh1Datas[MESH_1_DATA_BEAST_SEWON].init(resolveAssetsPath("Meshes/Heroes/Beast/sewon/Sewon.model"));
	m_mesh1Datas[MESH_1_DATA_FIGHTER_DAN_MEI].init(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/DanMei_A_Type.model"));

	// Assets anim data
	{
		const int numAnimFiles = 5;
		SkinnedMesh1::AnimFile animFiles[numAnimFiles] =
		{
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/cast.anim"), "attack_1"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/idle.anim"), "idle"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/la.anim"), "attack_2"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/la2.anim"), "attack_3"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/run.anim"), "run"),
		};
		m_anim1Datas[ANIM_1_DATA_BEAST_SEWON].init(animFiles, numAnimFiles);
	}

	{
		const int numAnimFiles = 5;
		SkinnedMesh1::AnimFile animFiles[numAnimFiles] =
		{
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/fwd.anim"), "run"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/idle.anim"), "idle"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/la.anim"), "attack_1"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/la2.anim"), "attack_2"),
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

	Material material;

	material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	material.Shininess = 16.0f;

	m_skinnedMeshes[SKINNED_MESH_MY_HERO_1].init(m_mesh1Datas[MESH_1_DATA_FIGHTER_DAN_MEI], m_anim1Datas[ANIM_1_DATA_FIGHTER_DAN_MEI], m_meshTextures[TEXTURES_MESH_FIGHTER_DAN_MEI], skinnedShader, &material);
	m_skinnedMeshes[SKINNED_MESH_ENEMY_HERO_1].init(m_mesh1Datas[MESH_1_DATA_BEAST_SEWON], m_anim1Datas[ANIM_1_DATA_BEAST_SEWON], m_meshTextures[TEXTURES_MESH_BEAST_SEWON], skinnedShader, &material);

	// Heroes
	m_heroes[0]->init(m_skinnedMeshes[SKINNED_MESH_MY_HERO_1], MyVec3(-25.0f, 0, -8.0f), MyVec3(0, 90, 0), MyVec3(0.015f), myBloodBar, MyVec3(-1, 6, 0));
	m_heroes[1]->init(m_skinnedMeshes[SKINNED_MESH_ENEMY_HERO_1], MyVec3(17.4f, 0, -1.0f), MyVec3(0, -90, 0), MyVec3(0.01f), enemyBloodBar, MyVec3(0, 6, 0));

	// Fill into list of living entities
	for (size_t i = 0; i < MAX_NUM_HEROES; i++)
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

//===================================================================================================================
//
// Hero state idle
//
//===================================================================================================================

void HeroState_Idle::Enter(Hero* hero)
{
	hero->changeAnimAction("Idle");
}

void HeroState_Idle::Execute(Hero* hero)
{
	if (hero->isMoving())
	{
		hero->getFSM()->ChangeState(HeroState_Walk::instance());
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
	hero->changeAnimAction("Walk");
}

void HeroState_Walk::Execute(Hero* hero)
{
	if (!hero->isMoving())
	{
		hero->getFSM()->ChangeState(HeroState_Idle::instance());
	}
}

void HeroState_Walk::Exit(Hero* hero)
{

}