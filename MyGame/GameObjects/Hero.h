
#pragma once

#include <MySkinnedMesh1.h>
#include <MyEventListener.h>
#include "LivingEntity.h"
#include "MovingEntity.h"
#include "Item.h"

//===================================================================================================================
//
// Structs
//
//===================================================================================================================

struct HeroProps
{
	int InitialMaxHealth;
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


#pragma region Constants

//===================================================================================================================
//
// Constants
//
//===================================================================================================================
const std::vector<float> EXP_LEVEL =
{
	1000,
	2000,
	4000,
	8000,
	16000,
	32000,
	64000,
	128000,
	256000,
	512000,
	1024000,
};

const float MONEY_PAWN = 50;
const float MONEY_TOWER = 400;
const float EXP_PAWN = 100;
const float EXP_TOWER = 500;

#pragma endregion


//===================================================================================================================
//
// Hero class
//
//===================================================================================================================

const int N_MAX_ITEM = 6;

class Hero : public LivingEntity
{
public:
	Hero();
	virtual ~Hero();

	// Core functions
	virtual void init(
		SkinnedMesh1& mesh,
		BloodBar& bloodBar, 
		std::vector<LivingEntity*>& lEnts,
		HeroProps& heroProp,
		HeroInGameProps& heroInGameProp,
		TEAM_TYPE team);
	virtual void update(Timer& timer);
	virtual void render(SpriteBatch& spriteBatch, Camera& camera, Light& light);

	MyVec3 getPos();
	int getGold();

	int findIndexForNewItem();
	void sellAnItem(int index);
	int buyAnItem(Item* item);

protected:
	virtual void dead();
	void revival();

protected:
	// Mesh/Appearance elements
	SkinnedMesh1::Instance* m_instance;

	// Moving elements
	MovingEntity m_movingEnt;

	float m_time_PAA_Attack_1;

	int m_exp;
	int m_gold;
	float m_countTime;
	float m_revivalTime;

	MyVec3 m_positionStart;
	MyVec3 m_rotationStart;

	Item* m_lItems[N_MAX_ITEM];
};

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

public:
	HeroPool();
	~HeroPool();

	void init(
		Shader& skinnedShader, 
		BloodBar& myBloodBar, 
		BloodBar& enemyBloodBar, 
		std::vector<LivingEntity*>& lEnts, 
		OnPressListenee& map);
	void update(Timer& timer);
	void render(Camera& camera, Light& light);

	Hero* getPlayer();

private:
	// Assets
	FileMesh1::MeshData     m_mesh1Datas[NUM_MESH_1_DATAS];
	SkinnedMesh1::AnimData  m_anim1Datas[NUM_ANIM_1_DATAS];
	FileMesh1::MeshTextures m_meshTextures[NUM_TEXTURES_MESHES];

	// Meshes
	SkinnedMesh1 m_skinnedMeshes[NUM_SKINNED_MESHES];
	Hero*        m_heroes[MAX_NUM_HEROES_IN_GAME];
};