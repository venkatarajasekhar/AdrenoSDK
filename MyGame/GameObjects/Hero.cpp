#include "HeroItem.h"
#include "Hero.h"
#include "Hero_AI.h"
#include "Hero_Controlled.h"

#pragma region Hero properties

//===================================================================================================================
//
// Hero properties
//
//===================================================================================================================

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
	g_HeroProps[HERO_BEAST_SEWON].InitialMaxHealth = 200;
	g_HeroProps[HERO_BEAST_SEWON].InitialDamage = 20;

	g_HeroProps[HERO_BEAST_SEWON].AttackRange = 7;
	g_HeroProps[HERO_BEAST_SEWON].ChasingRange = 10;

	g_HeroProps[HERO_BEAST_SEWON].MovingSpeed = 5.0f;
	g_HeroProps[HERO_BEAST_SEWON].MovingRotYOffset = 0;
	g_HeroProps[HERO_BEAST_SEWON].MovingTurnSpeed = 1000;

	g_HeroProps[HERO_BEAST_SEWON].BloodbarOffset = MyVec3(-2, 5, 0);

	g_HeroProps[HERO_BEAST_SEWON].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_HeroProps[HERO_BEAST_SEWON].MeshMaterial.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_HeroProps[HERO_BEAST_SEWON].MeshMaterial.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_HeroProps[HERO_BEAST_SEWON].MeshMaterial.Shininess = 16.0f;

	g_HeroProps[HERO_BEAST_SEWON].Time_PAA_Attack_1 = 0.63768f;

	// Fighter dan mei
	g_HeroProps[HERO_FIGHTER_DAN_MEI].InitialMaxHealth = 200;
	g_HeroProps[HERO_FIGHTER_DAN_MEI].InitialDamage = 20;

	g_HeroProps[HERO_FIGHTER_DAN_MEI].AttackRange = 7;
	g_HeroProps[HERO_BEAST_SEWON].ChasingRange = 10;

	g_HeroProps[HERO_FIGHTER_DAN_MEI].MovingSpeed = 5.0f;
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MovingRotYOffset = 0;
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MovingTurnSpeed = 1000;

	g_HeroProps[HERO_FIGHTER_DAN_MEI].BloodbarOffset = MyVec3(-1, 5, 0);

	g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial.Shininess = 16.0f;

	g_HeroProps[HERO_FIGHTER_DAN_MEI].Time_PAA_Attack_1 = 0.3636f;
}

#pragma endregion

#pragma region Hero in-game properties

//===================================================================================================================
//
// Hero in-game properties
//
//===================================================================================================================

static HeroInGameProps g_HeroInGameProps[HeroPool::MAX_NUM_HEROES_IN_GAME];

static void initHeroInGameProps()
{
	// My hero 1
	g_HeroInGameProps[HeroPool::HERO_IN_GAME_MY_HERO_1].Pos = MyVec3(-40.0f, 0, 0.0f);
	g_HeroInGameProps[HeroPool::HERO_IN_GAME_MY_HERO_1].Rot = MyVec3(0, 90, 0);
	g_HeroInGameProps[HeroPool::HERO_IN_GAME_MY_HERO_1].Scale = MyVec3(0.011f);

	// Enemy hero 1
	g_HeroInGameProps[HeroPool::HERO_IN_GAME_ENEMY_HERO_1].Pos = MyVec3(40.0f, 0, 0.0f);
	g_HeroInGameProps[HeroPool::HERO_IN_GAME_ENEMY_HERO_1].Rot = MyVec3(0, -90, 0);
	g_HeroInGameProps[HeroPool::HERO_IN_GAME_ENEMY_HERO_1].Scale = MyVec3(0.006f);
}

#pragma endregion

#pragma region Constants

//===================================================================================================================
//
// Constants
//
//===================================================================================================================

static const MyVec2 HERO_BLOOD_BAR_SCALE = MyVec2(1.5f, 0.8f);

static const std::vector<MyVec3> ENEMY_HERO_PATH =
{
	MyVec3(40.0f, 0, 0.0f),
	MyVec3(30.6013f, 0, 2.89223f),
	MyVec3(14.6958f, 0, -3.19085f),
	MyVec3(0.451345f, 0, -1.58377f),
	MyVec3(-12.2037f, 0, 3.04689f),
	MyVec3(-23.3116f, 0, -4.71635f),
	MyVec3(-30.7036f, 0, -4.28849f),
	MyVec3(-37.9492f, 0, -0.463198f),
};

static const float RANGE_OF_MAIN_TOWER = 10.0f;
static const int HEALTH_PER_SECOND_IN_RANGE_MAINTOWER = 20;
static const int HEALTH_PER_SECOND_OUT_RANGE_MAINTOWER = 1;
static const MyVec3 POSITION_MY_MAIN_TOWER = MyVec3(-43.0f, 0, 0.39f);
static const MyVec3 POSITION_ENEMY_MAIN_TOWER = MyVec3(39.0f, 0, -1.5f);

#pragma endregion

//===================================================================================================================
//
// Hero class
//
//===================================================================================================================

Hero::Hero()
	: m_instance(nullptr)
{
	m_itemBag.reserve(MAX_NUM_ITEMS);
}

Hero::~Hero()
{
	for (auto i = m_itemBag.begin(); i != m_itemBag.end(); ++i)
	{
		delete(*i);
	}

	for (auto i = m_skillBag.begin(); i != m_skillBag.end(); ++i)
	{
		delete(*i);
	}
}

// Core functions

void Hero::init(
	SkinnedMesh1& mesh,
	BloodBar& bloodBar,
	std::vector<LivingEntity*>& lEnts,
	HeroProps& heroProp,
	HeroInGameProps& heroInGameProp,
	TEAM_TYPE team)
{
	m_time_PAA_Attack_1 = heroProp.Time_PAA_Attack_1;

	// Mesh/Appearance elements
	m_instance = SkinnedMesh1::buildSkinnedMeshInstance(heroInGameProp.Pos, heroInGameProp.Rot, heroInGameProp.Scale, "idle");
	mesh.addInstance(m_instance);

	setTeamType(team);
	setEntityType(ENTITY_TYPE_HERO);

	m_inUse = true;

	m_exp = 0;
	m_gold = 600;
	m_healthPerSecond = HEALTH_PER_SECOND_OUT_RANGE_MAINTOWER;
	m_healthPerAttack = 0;
	m_countTime = 0;
	m_revivalTime = 0;

	m_positionStart = heroInGameProp.Pos;
	m_rotationStart = heroInGameProp.Rot;

	LivingEntity::init(
		heroProp.InitialMaxHealth,
		heroProp.InitialDamage,
		bloodBar, 
		HERO_BLOOD_BAR_SCALE, 
		heroProp.BloodbarOffset,
		lEnts, 
		heroProp.AttackRange);
}

void Hero::update(Timer& timer)
{
	// Moving elements
	m_movingEnt.update(timer);

	m_instance->Position = m_movingEnt.getPos();
	m_instance->Rotation = m_movingEnt.getRot();

	m_countTime += timer.getElapsedTime();
	if (m_countTime >= 1.0f)
	{
		if (m_teamType == TEAM_TYPE_MY_TEAM)
		{
			if (distance_optimized(getPos(), POSITION_MY_MAIN_TOWER) <= RANGE_OF_MAIN_TOWER)
				m_healthPerSecond = HEALTH_PER_SECOND_IN_RANGE_MAINTOWER;
			else m_healthPerSecond = HEALTH_PER_SECOND_OUT_RANGE_MAINTOWER;
		}
		else
		{
			if (distance_optimized(getPos(), POSITION_ENEMY_MAIN_TOWER) <= RANGE_OF_MAIN_TOWER)
				m_healthPerSecond = HEALTH_PER_SECOND_IN_RANGE_MAINTOWER;
			else m_healthPerSecond = HEALTH_PER_SECOND_OUT_RANGE_MAINTOWER;
		}

		if (m_instance->Visible) accHealth(m_healthPerSecond);
		m_gold++;
		m_countTime--;
	}

	if (!m_instance->Visible)
		m_revivalTime += timer.getElapsedTime();
	if ((!m_instance->Visible) && (m_revivalTime >= 5))
	{
		m_revivalTime = 0;
		revival();
	}

	/*for (auto i = m_itemBag.begin(); i != m_itemBag.end(); ++i)
	{
		delete(*i);
		//m_itemBag.erase(i);
	}*/

	for (auto i = m_itemBag.begin(); i != m_itemBag.end(); ++i)
	{
		//((HeroItem*)(*i))->useItem();
		((HeroItem*)(*i))->update(timer, this);
	}

}

void Hero::render(SpriteBatch& spriteBatch, Camera& camera, Light& light)
{
	if (m_instance->Visible)
	{
		LivingEntity::render(spriteBatch, camera, light);

		for (auto i = m_skillBag.begin(); i != m_skillBag.end(); ++i)
		{
			(*i)->render(camera);
		}
	}
}

MyVec3 Hero::getPos()
{
	return m_movingEnt.getPos();
}

void Hero::accPos(MyVec3 dPos)
{
	m_movingEnt.setPos(m_movingEnt.getPos() + dPos);
}

MyVec3 Hero::getRot()
{
	return m_movingEnt.getRot() + MyVec3(0, m_movingEnt.getRotYOffset(), 0);
}

int Hero::getGold()
{
	return m_gold;
}

void Hero::setHealthPerAttack(int health)
{
	m_healthPerAttack = health;
}

void Hero::addItem(HeroItem* item)
{
	if (item != nullptr)
	{
		if (m_itemBag.size() < MAX_NUM_ITEMS)
		{
			m_itemBag.push_back(item);
		}
		else
		{
			delete item;
		}
	}
}

void Hero::addSkill(HeroSkill* skill)
{
	if (skill != nullptr)
	{
		m_skillBag.push_back(skill);
	}
}

void Hero::OnBuyItemItem(const IOnBuyItemListener::Data& data)
{
	//addItem(data.BoughtItem->clone());
	addItem(data.BoughtItem);
}

void Hero::dead()
{
	m_instance->Visible = false;
	
	if (getTeamType() == TEAM_TYPE_ENEMY)
		m_movingEnt.setPath(ENEMY_HERO_PATH);
	else
	{
		m_movingEnt.setTarget(m_positionStart);
		m_movingEnt.setPos(m_positionStart);
		m_movingEnt.setRot(m_rotationStart);
	}
}

void Hero::revival()
{	
	m_health = m_maxHealth;
	m_instance->Visible = true;
	m_inUse = true;
}

#pragma region HeroPool class

//===================================================================================================================
//
// HeroPool class
//
//===================================================================================================================

HeroPool::HeroPool()
{
	for (int i = 0; i < MAX_NUM_HEROES_IN_GAME; i++)
	{
		m_heroes[i] = nullptr;
	}
}

HeroPool::~HeroPool()
{
	for (int i = 0; i < MAX_NUM_HEROES_IN_GAME; i++)
	{
		SAFE_DELETE(m_heroes[i]);
	}
}

void HeroPool::init(
	Shader& skinnedShader, 
	Shader& billboardShader,
	BloodBar& myBloodBar, 
	BloodBar& enemyBloodBar, 
	std::vector<LivingEntity*>& lEnts, 
	OnPressListenee& map)
{
	initHeroProps();
	initHeroInGameProps();

	// Assets texture
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/skill.pak").c_str());

		m_textures[TEXTURE_SKILL_BATTLE_BORN].init(resource.GetTexture("Battle_Born"));
		m_textures[TEXTURE_SKILL_BLADEFALL].init(resource.GetTexture("Bladefall"));
		m_textures[TEXTURE_SKILL_DECIMATION_DAY].init(resource.GetTexture("Decimation_Day"));
		m_textures[TEXTURE_SKILL_JUST_DESSERTS].init(resource.GetTexture("Just_Desserts"));

		m_spriteSheets[SPRITE_SHEET_SKILL_BATTLE_BORN].init(resource.GetTexture("Battle_Born_Effect"), 8, MyIVec2(5, 3), MyIVec2(139, 153));
	}

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
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/la2.anim"), "attack_1"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/cast.anim"), "attack_2"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Beast/sewon/la.anim"), "attack_3"),
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

	// Graphics objects
	{
		float width = 5.0f;
		m_billboards[BILLBOARD_SKILL_BATTLE_BORN].init(
			&m_spriteSheets[SPRITE_SHEET_SKILL_BATTLE_BORN],
			billboardShader,
			MyVec3(0),
			MyVec2(width, width * (153.0f / 139.0f)),
			0);
	}
	
	// Skinned meshes
	m_skinnedMeshes[SKINNED_MESH_BEAST_SEWON].init(
		m_mesh1Datas[MESH_1_DATA_BEAST_SEWON], 
		m_anim1Datas[ANIM_1_DATA_BEAST_SEWON], 
		m_meshTextures[TEXTURES_MESH_BEAST_SEWON], 
		skinnedShader, 
		&g_HeroProps[HERO_BEAST_SEWON].MeshMaterial,
		0.8f);
	m_skinnedMeshes[SKINNED_MESH_FIGHTER_DAN_MEI].init(
		m_mesh1Datas[MESH_1_DATA_FIGHTER_DAN_MEI], 
		m_anim1Datas[ANIM_1_DATA_FIGHTER_DAN_MEI], 
		m_meshTextures[TEXTURES_MESH_FIGHTER_DAN_MEI], 
		skinnedShader, 
		&g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial);

	// Adjusting unaligned action
	m_skinnedMeshes[SKINNED_MESH_FIGHTER_DAN_MEI].translateAction("attack_1", MyVec3(0, 0, -200));
	m_skinnedMeshes[SKINNED_MESH_BEAST_SEWON].translateAction("attack_1", MyVec3(0, 0, -400));

	// Heroes
	{
		Hero_Controlled* hero = new Hero_Controlled;
		hero->init(
			m_skinnedMeshes[SKINNED_MESH_FIGHTER_DAN_MEI], 
			myBloodBar, 
			lEnts, 
			g_HeroProps[HERO_FIGHTER_DAN_MEI], 
			g_HeroInGameProps[HeroPool::HERO_IN_GAME_MY_HERO_1],
			TEAM_TYPE_MY_TEAM);

		hero->addSkill(new HeroSkill_BattleBorn("Battle Born", 0, 0, 0, &m_textures[TEXTURE_SKILL_BATTLE_BORN], &m_billboards[BILLBOARD_SKILL_BATTLE_BORN]));
		hero->addSkill(new HeroSkill_Bladefall("Bladefall", 0, 0, 0, &m_textures[TEXTURE_SKILL_BLADEFALL]));
		hero->addSkill(new HeroSkill_DecimationDay("Decimation Day", 0, 0, 0, &m_textures[TEXTURE_SKILL_DECIMATION_DAY]));
		hero->addSkill(new HeroSkill_JustDesserts("Just Desserts", 0, 0, 0, &m_textures[TEXTURE_SKILL_JUST_DESSERTS]));

		m_heroes[HERO_IN_GAME_MY_HERO_1] = hero;

		map.addPressListener(hero);
	}
	{
		Hero_AI* hero = new Hero_AI;
		hero->init(
			m_skinnedMeshes[SKINNED_MESH_BEAST_SEWON], 
			enemyBloodBar, 
			ENEMY_HERO_PATH, 
			lEnts, 
			g_HeroProps[HERO_BEAST_SEWON], 
			g_HeroInGameProps[HeroPool::HERO_IN_GAME_ENEMY_HERO_1],
			TEAM_TYPE_ENEMY);

		m_heroes[HERO_IN_GAME_ENEMY_HERO_1] = hero;
	}

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

	// Assets
	for (int i = 0; i < NUM_SPRITE_SHEETS; i++)
	{
		m_spriteSheets[i].update(timer);
	}

	// Graphics objects
	for (int i = 0; i < NUM_BILLBOARDS; i++)
	{
		m_billboards[i].update(timer);
	}
}

void HeroPool::render(Camera& camera, Light& light)
{
	for (int i = 0; i < NUM_SKINNED_MESHES; i++)
	{
		m_skinnedMeshes[i].render(camera, &light);
	}
}

Hero* HeroPool::getPlayer()
{
	return m_heroes[HERO_IN_GAME_MY_HERO_1];
}

#pragma endregion