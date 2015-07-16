
#pragma once

#include <MySkinnedMesh1.h>
#include <MyEventListener.h>
#include <MyBillboard.h>
#include <MySpriteSheet.h>
#include "LivingEntity.h"
#include "MovingEntity.h"
#include "EventListener.h"
#include "HeroSkill.h"
#include <MyAudio.h>

#pragma region Structs

//===================================================================================================================
//
// Structs
//
//===================================================================================================================

struct HeroProps
{
	int InitialMaxHealth;
	int InitialMaxMana;
	int InitialDamage;

	float AttackRange;
	float ChasingRange;

	float MovingSpeed;
	float MovingRotYOffset;
	float MovingTurnSpeed;

	MyVec3 BloodbarOffset;

	Material MeshMaterial;

	float Time_PAA_Attack_1;
};

struct HeroInGameProps
{
	MyVec3 Pos;
	MyVec3 Rot;
	MyVec3 Scale;
};

#pragma endregion

#pragma region Constants

//===================================================================================================================
//
// Constants
//
//===================================================================================================================
const std::vector<float> EXP_LEVEL =
{
	0,
	500,
	1000,
	1600,
	2200,
	2900,
	3600,
	4400,
	5200,
	6100,
	7000,
	8000,
	9000,
};

#pragma endregion

class HeroItem;

//===================================================================================================================
//
// Hero class
//
//===================================================================================================================

class Hero : public LivingEntity, public IOnBuyItemListener
{
public:
	static const int MAX_NUM_ITEMS = 6;

public:

	enum
	{
		AUDIO_MYPAWN_ATTACK,
		AUDIO_MYPAWN_DEATH,
		AUDIO_ENEMYPAWN_ATTACK,
		AUDIO_ENEMYPAWN_DEATH,
		AUDIO_MYIFV_ATTACK,
		AUDIO_ENEMYIFV_ATTACK,
		AUDIO_IFV_DEATH,
		AUDIO_MYTOWER_ATTACK,
		AUDIO_ENEMYTOWER_ATTACK,
		AUDIO_TOWER_DEATH,
		AUDIO_MYHERO_ATTACK,
		AUDIO_MYHERO_DEATH,
		AUDIO_ENEMYHERO_ATTACK,
		AUDIO_ENEMYHERO_ATTACK2,
		AUDIO_ENEMYHERO_DEATH,
		AUDIO_HEALTH_SKILL,
		AUDIO_UPGRADE_SKILL,
		AUDIO_SHOP_OPEN,
		AUDIO_BACKGROUND,
		NUM_AUDIOS,
	};

	Hero();
	virtual ~Hero();

	// Core functions
	virtual void init(
		SkinnedMesh1& mesh,
		BloodBar& bloodBar,
		Quad3D& selectedDecal,
		std::vector<LivingEntity*>& lEnts,
		Audio lAudios[],
		HeroProps& heroProp,
		HeroInGameProps& heroInGameProp,
		TEAM_TYPE team);
	virtual void update(Timer& timer);
	void update2(Timer& timer);
	virtual void render(SpriteBatch& spriteBatch, Camera& camera, Light& light);

	virtual void beginMatch();

	MyVec3 getPos();
	MyVec3 getRot();
	void setHealthPerAttack(int health);
	void setTarget(MyVec3 target);
	void accPos(MyVec3 dPos);
	void accMana(int mana);
	void accMaxMana(int mana);
	void accGold(int gold);
	void accSpeed(float speed);
	int getGold();
	int getMana();
	int getMaxMana();
	int getExp();
	int getMaxExp();
	int getLevel();
	Audio* getAudio(int id);
	bool getVisible();
	int getGoldLost();
	int getExpLost();

	void levelUp(int newLevel);
	void addItem(HeroItem* item);
	void addSkill(HeroSkill* skill);

	virtual void useSkill(int skillID){}

	std::vector<HeroSkill*>& getSkillBag(){ return m_skillBag; }

	void OnBuyItemItem(const IOnBuyItemListener::Data& data);

	void turnToTarget();

protected:
	virtual void dead();
	void revival();

protected:
	// Mesh/Appearance elements
	SkinnedMesh1::Instance* m_instance;

	// Moving elements
	MovingEntity m_movingEnt;

	float m_time_PAA_Attack_1;

	int m_mana;
	int m_maxMana;
	int m_level;
	int m_gold;
	int m_exp;
	int m_healthPerSecond;
	int m_manaPerSecond;
	int m_healthPerAttack;
	float m_countTime;
	float m_revivalTime;

	MyVec3 m_positionStart;
	MyVec3 m_rotationStart;

	std::vector<HeroItem*> m_itemBag;
	std::vector<HeroSkill*> m_skillBag;

	Audio* m_audios;
};

#pragma region HeroPool class

//===================================================================================================================
//
// HeroPool class
//
//===================================================================================================================

class HeroPool
{
public:
	// Heroes in-game
	enum
	{
		HERO_IN_GAME_MY_HERO_1,
		HERO_IN_GAME_ENEMY_HERO_1,
		MAX_NUM_HEROES_IN_GAME,
	};

private:
	// Assets
	enum
	{
		TEXTURE_SKILL_BATTLE_BORN,
		TEXTURE_SKILL_BLADEFALL,
		TEXTURE_SKILL_DECIMATION_DAY,
		TEXTURE_SKILL_JUST_DESSERTS,
		TEXTURE_SKILL_EFFECT_BATTLE_BORN,
		TEXTURE_SKILL_EFFECT_BLADEFALL,
		TEXTURE_SKILL_EFFECT_DECIMATION_DAY,
		NUM_TEXTURES,
	};

	enum
	{
		MESH_1_DATA_BEAST_SEWON,
		MESH_1_DATA_FIGHTER_DAN_MEI,
		NUM_MESH_1_DATAS,
	};

	enum
	{
		ANIM_1_DATA_BEAST_SEWON,
		ANIM_1_DATA_FIGHTER_DAN_MEI,
		NUM_ANIM_1_DATAS,
	};

	enum
	{
		TEXTURES_MESH_BEAST_SEWON,
		TEXTURES_MESH_FIGHTER_DAN_MEI,
		NUM_TEXTURES_MESHES,
	};

	// Meshes
	enum
	{
		SKINNED_MESH_BEAST_SEWON,
		SKINNED_MESH_FIGHTER_DAN_MEI,
		NUM_SKINNED_MESHES,
	};

	// Effects object
	enum
	{
		SPHERE_BATTLE_BORN,
		SPHERE_BLADE_FALL,
		SPHERE_DECIMATION_DAY,
		NUM_SPHERES,
	};

public:
	HeroPool();
	~HeroPool();

	void init(
		Shader& skinnedShader, 
		Shader& billboardShader,
		Shader& shapeShader,
		BloodBar& myBloodBar, 
		BloodBar& enemyBloodBar, 
		Quad3D& selectedDecal,
		std::vector<LivingEntity*>& lEnts,
		Audio lAudios[],
		OnPressListenee& map);
	void update(Timer& timer);
	void render(Camera& camera, Light& light);

	Hero* getPlayer();

private:
	// Assets
	Texture                 m_textures[NUM_TEXTURES];
	FileMesh1::MeshData     m_mesh1Datas[NUM_MESH_1_DATAS];
	SkinnedMesh1::AnimData  m_anim1Datas[NUM_ANIM_1_DATAS];
	FileMesh1::MeshTextures m_meshTextures[NUM_TEXTURES_MESHES];

	// Meshes
	SkinnedMesh1 m_skinnedMeshes[NUM_SKINNED_MESHES];
	Hero*        m_heroes[MAX_NUM_HEROES_IN_GAME];

	// Effects objects
	Sphere m_skillEffect[NUM_SPHERES];
};

#pragma endregion