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
	g_HeroProps[HERO_BEAST_SEWON].InitialMaxHealth = 550;
	g_HeroProps[HERO_BEAST_SEWON].InitialMaxMana = 250;
	g_HeroProps[HERO_BEAST_SEWON].InitialDamage = 60;

	g_HeroProps[HERO_BEAST_SEWON].AttackRange = 5;
	g_HeroProps[HERO_BEAST_SEWON].ChasingRange = 15;

	g_HeroProps[HERO_BEAST_SEWON].MovingSpeed = 5.0f;
	g_HeroProps[HERO_BEAST_SEWON].MovingRotYOffset = 0;
	g_HeroProps[HERO_BEAST_SEWON].MovingTurnSpeed = 1000;

	g_HeroProps[HERO_BEAST_SEWON].BloodbarOffset = MyVec3(-2, 5, 0);

#ifdef WIN32
	g_HeroProps[HERO_BEAST_SEWON].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_HeroProps[HERO_BEAST_SEWON].MeshMaterial.Diffuse = MyVec4(1.0f, 0.0f, 1.0f, 1.0f);
	g_HeroProps[HERO_BEAST_SEWON].MeshMaterial.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_HeroProps[HERO_BEAST_SEWON].MeshMaterial.Shininess = 16.0f;
#elif defined __ANDROID__
	g_HeroProps[HERO_BEAST_SEWON].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_HeroProps[HERO_BEAST_SEWON].MeshMaterial.Diffuse = MyVec4(0.0f, 1.0f, 1.0f, 1.0f);
	g_HeroProps[HERO_BEAST_SEWON].MeshMaterial.Specular = MyVec4(0.0f, 0.0f, 0.0f, 1.0f);
	g_HeroProps[HERO_BEAST_SEWON].MeshMaterial.Shininess = 1.0f;
#endif
	
	g_HeroProps[HERO_BEAST_SEWON].Time_PAA_Attack_1 = 0.63768f;

	// Fighter dan mei
	g_HeroProps[HERO_FIGHTER_DAN_MEI].InitialMaxHealth = 600;
	g_HeroProps[HERO_FIGHTER_DAN_MEI].InitialMaxMana = 250;
	g_HeroProps[HERO_FIGHTER_DAN_MEI].InitialDamage = 50;
	//g_HeroProps[HERO_FIGHTER_DAN_MEI].InitialDamage = 500;

	g_HeroProps[HERO_FIGHTER_DAN_MEI].AttackRange = 7;
	g_HeroProps[HERO_BEAST_SEWON].ChasingRange = 15;

	g_HeroProps[HERO_FIGHTER_DAN_MEI].MovingSpeed = 5.0f;
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MovingRotYOffset = 0;
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MovingTurnSpeed = 1000;

	g_HeroProps[HERO_FIGHTER_DAN_MEI].BloodbarOffset = MyVec3(-1, 5, 0);

#ifdef WIN32
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial.Diffuse = MyVec4(1.0f, 0.0f, 1.0f, 1.0f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial.Shininess = 16.0f;
#elif defined __ANDROID__
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial.Diffuse = MyVec4(1.0f, 0.0f, 1.0f, 1.0f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial.Specular = MyVec4(0.001f, 0.001f, 0.001f, 1.0f);
	g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial.Shininess = 2.0f;
#endif
	
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
	MyVec3(34.0f, 0, 0.0f),
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
static const int MANA_PER_SECOND_IN_RANGE_MAINTOWER = 20;
static const int MANA_PER_SECOND_OUT_RANGE_MAINTOWER = 1;
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
	Quad3D& selectedDecal,
	std::vector<LivingEntity*>& lEnts,
	Audio lAudios[],
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
	m_gold = 1000;
	m_maxMana = heroProp.InitialMaxMana;
	m_mana = m_maxMana;
	m_level = 1;
	m_healthPerSecond = HEALTH_PER_SECOND_IN_RANGE_MAINTOWER;
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
		heroProp.AttackRange,
		&selectedDecal);

	m_audios = lAudios;
}

void Hero::update(Timer& timer)
{
	m_movingEnt.update(timer);

	m_instance->Position = m_movingEnt.getPos();
	m_instance->Rotation = m_movingEnt.getRot();

	m_countTime += timer.getElapsedTime();
	if (m_countTime >= 1.0f)
	{
		if (m_teamType == TEAM_TYPE_MY_TEAM)
		{
			if (distance_optimized(getPos(), POSITION_MY_MAIN_TOWER) <= RANGE_OF_MAIN_TOWER)
			{
				m_healthPerSecond = HEALTH_PER_SECOND_IN_RANGE_MAINTOWER;
				m_manaPerSecond = MANA_PER_SECOND_IN_RANGE_MAINTOWER;
			}
			else
			{
				m_healthPerSecond = HEALTH_PER_SECOND_OUT_RANGE_MAINTOWER;
				m_manaPerSecond = MANA_PER_SECOND_OUT_RANGE_MAINTOWER;
			}
		}
		else
		{
			if (distance_optimized(getPos(), POSITION_ENEMY_MAIN_TOWER) <= RANGE_OF_MAIN_TOWER)
			{
				m_healthPerSecond = HEALTH_PER_SECOND_IN_RANGE_MAINTOWER;
				m_manaPerSecond = MANA_PER_SECOND_IN_RANGE_MAINTOWER;
			}
			else
			{
				m_healthPerSecond = HEALTH_PER_SECOND_OUT_RANGE_MAINTOWER;
				m_manaPerSecond = MANA_PER_SECOND_OUT_RANGE_MAINTOWER;
			}
		}

		if (m_instance->Visible)
		{
			accHealth(m_healthPerSecond);
			accMana(m_manaPerSecond);
		}
		m_gold++;
		m_countTime--;
	}

	if (!m_instance->Visible)
		m_revivalTime += timer.getElapsedTime();
	if ((!m_instance->Visible) && (m_revivalTime >= m_level*4))
	{
		m_revivalTime = 0;
		revival();
	}

	/*for (auto i = m_itemBag.begin(); i != m_itemBag.end(); ++i)
	{
		if (((*i)->getNTime() == 0) && (!(*i)->getIsUsing()))
		{
			delete(*i);
			m_itemBag.erase(i);
			break;
		}
	}*/

	for (auto i = m_itemBag.begin(); i != m_itemBag.end(); ++i)
	{
		((HeroItem*)(*i))->update(timer, this);
	}

	for (auto i = m_skillBag.begin(); i != m_skillBag.end(); ++i)
	{
		(*i)->update(timer);
	}

	if (m_exp >= EXP_LEVEL[m_level])
	{
		for (int i = 0; i < EXP_LEVEL.size(); i++)
		{
			if (EXP_LEVEL[i] > m_exp)
			{
				levelUp(i);
				break;
			}
		}
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

void Hero::beginMatch()
{
	m_health = m_maxHealth;
	m_selected = false;
	m_inUse = true;
	m_instance->Visible = true;

	m_exp = 0;
	m_gold = 5000;
	m_mana = m_maxMana;
	m_level = 1;
	m_healthPerSecond = HEALTH_PER_SECOND_IN_RANGE_MAINTOWER;
	m_healthPerAttack = 0;
	m_countTime = 0;
	m_revivalTime = 0;
}

MyVec3 Hero::getPos()
{
	return m_movingEnt.getPos();
}

void Hero::accPos(MyVec3 dPos)
{
	m_movingEnt.setPos(m_movingEnt.getPos() + dPos);
}

void Hero::accMana(int mana)
{
	m_mana += mana;
	if (m_mana > m_maxMana) m_mana = m_maxMana;
}

void Hero::accMaxMana(int mana)
{
	m_maxMana += mana;
	if (m_maxMana < 0) m_maxMana = 0;
}


void Hero::accGold(int gold)
{
	m_gold += gold;
}

void Hero::accSpeed(float speed)
{
	m_movingEnt.setSpeed(m_movingEnt.getSpeed() + speed);
}

MyVec3 Hero::getRot()
{
	return m_movingEnt.getRot() + MyVec3(0, m_movingEnt.getRotYOffset(), 0);
}

int Hero::getGold()
{
	return m_gold;
}

int Hero::getMana()
{
	return m_mana;
} 

int Hero::getMaxMana()
{
	return m_maxMana;
}

int Hero::getExp()
{
	return m_exp - EXP_LEVEL[m_level - 1];
}

int Hero::getMaxExp()
{
	return EXP_LEVEL[m_level] - EXP_LEVEL[m_level - 1];
}

int Hero::getLevel()
{
	return m_level;
}

Audio* Hero::getAudio(int id)
{
	return &m_audios[id];
}

void Hero::levelUp(int newLevel)
{
	m_audios[AUDIO_UPGRADE_SKILL].play();
	int nLevel = newLevel - m_level;
	m_health += nLevel * 100;
	m_maxHealth += nLevel * 100;
	m_damage += nLevel * 5;
	m_mana += nLevel * 20;
	m_maxMana += nLevel * 20;
	m_level = newLevel;
}

void Hero::setHealthPerAttack(int health)
{
	m_healthPerAttack = health;
}

void Hero::setTarget(MyVec3 target)
{
	m_movingEnt.setTarget(target);
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
}

void Hero::revival()
{	
	m_health = m_maxHealth;
	m_instance->Visible = true;
	m_inUse = true;
}

void Hero::turnToTarget()
{
	m_movingEnt.turnTo(m_atkTarget->getPos());
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
	Shader& shapeShader,
	BloodBar& myBloodBar, 
	BloodBar& enemyBloodBar,
	Quad3D& selectedDecal,
	std::vector<LivingEntity*>& lEnts, 
	Audio lAudios[],
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

		m_textures[TEXTURE_SKILL_EFFECT_BATTLE_BORN].init(resource.GetTexture("Battle_Born_Effect"));
		m_textures[TEXTURE_SKILL_EFFECT_BLADEFALL].init(resource.GetTexture("Bladefall_Effect"));
		m_textures[TEXTURE_SKILL_EFFECT_DECIMATION_DAY].init(resource.GetTexture("Decimation_Day_Effect"));
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
		const int numAnimFiles = 6;
		SkinnedMesh1::AnimFile animFiles[numAnimFiles] =
		{
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/idle.anim"), "idle"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/fwd.anim"), "run"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/la2.anim"), "attack_1"), // Normal attack
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/llla.anim"), "attack_2"), // Skill attack 1
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/lose.anim"), "attack_3"), // Skill attack 2
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/Fighter/dan_mei/lra_power.anim"), "attack_4"), // Skill attack 3
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
		&g_HeroProps[HERO_BEAST_SEWON].MeshMaterial,
		0.8f);
	m_skinnedMeshes[SKINNED_MESH_FIGHTER_DAN_MEI].init(
		m_mesh1Datas[MESH_1_DATA_FIGHTER_DAN_MEI], 
		m_anim1Datas[ANIM_1_DATA_FIGHTER_DAN_MEI], 
		m_meshTextures[TEXTURES_MESH_FIGHTER_DAN_MEI], 
		skinnedShader, 
		&g_HeroProps[HERO_FIGHTER_DAN_MEI].MeshMaterial);

	// Adjusting unaligned action
	m_skinnedMeshes[SKINNED_MESH_FIGHTER_DAN_MEI].translateAction("attack_1", MyVec3(0, 0, -200)); // la2.anim
	m_skinnedMeshes[SKINNED_MESH_FIGHTER_DAN_MEI].translateAction("attack_2", MyVec3(0, 0, -650)); // llla.anim
	m_skinnedMeshes[SKINNED_MESH_FIGHTER_DAN_MEI].translateAction("attack_4", MyVec3(-100, 0, -100)); // lra_power.anim
	
	m_skinnedMeshes[SKINNED_MESH_BEAST_SEWON].translateAction("attack_1", MyVec3(0, 0, -400));

	// Effects objects
	m_skillEffect[SPHERE_BATTLE_BORN].init(shapeShader, &m_textures[TEXTURE_SKILL_EFFECT_BATTLE_BORN], MyVec3(0), MyVec3(0), MyVec3(3), 20, 20, MyVec3(100, 100, 200));
	m_skillEffect[SPHERE_BLADE_FALL].init(shapeShader, &m_textures[TEXTURE_SKILL_EFFECT_BLADEFALL], MyVec3(0), MyVec3(0, 0, -90), MyVec3(3), 20, 20);
	m_skillEffect[SPHERE_DECIMATION_DAY].init(shapeShader, &m_textures[TEXTURE_SKILL_EFFECT_DECIMATION_DAY], MyVec3(0), MyVec3(0, 0, -20), MyVec3(3), 20, 20);

	// Heroes
	{
		Hero_Controlled* hero = new Hero_Controlled;
		hero->init(
			m_skinnedMeshes[SKINNED_MESH_FIGHTER_DAN_MEI], 
			myBloodBar, 
			selectedDecal,
			lEnts, 
			lAudios,
			g_HeroProps[HERO_FIGHTER_DAN_MEI], 
			g_HeroInGameProps[HeroPool::HERO_IN_GAME_MY_HERO_1],
			TEAM_TYPE_MY_TEAM);

		hero->addSkill(new HeroSkill_BattleBorn("Battle Born", 0, 0, 3, &m_textures[TEXTURE_SKILL_BATTLE_BORN], &m_skillEffect[SPHERE_BATTLE_BORN]));
		hero->addSkill(new HeroSkill_Bladefall("Bladefall", 0, 0, 3, &m_textures[TEXTURE_SKILL_BLADEFALL], &m_skillEffect[SPHERE_BLADE_FALL]));
		hero->addSkill(new HeroSkill_DecimationDay("Decimation Day", 0, 0, 10, &m_textures[TEXTURE_SKILL_DECIMATION_DAY], &m_skillEffect[SPHERE_DECIMATION_DAY]));
		hero->addSkill(new HeroSkill_JustDesserts("Just Desserts", 0, 0, 10, &m_textures[TEXTURE_SKILL_JUST_DESSERTS]));

		m_heroes[HERO_IN_GAME_MY_HERO_1] = hero;

		map.addPressListener(hero);
	}
	{
		Hero_AI* hero = new Hero_AI;
		hero->init(
			m_skinnedMeshes[SKINNED_MESH_BEAST_SEWON], 
			enemyBloodBar, 
			selectedDecal,
			ENEMY_HERO_PATH, 
			lEnts, 
			lAudios,
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

	for (int i = 0; i < NUM_SPHERES; i++)
	{
		m_skillEffect[i].update(timer);
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