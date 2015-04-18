
#pragma once

#include <MySkinnedMesh1.h>
#include <MyEventListener.h>
#include "LivingEntity.h"
#include "MovingEntity.h"

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

	Material Material;

	float Time_PAA_Attack_1;
};

struct HeroInGameProps
{
	MyVec3 Pos;
	MyVec3 Rot;
	MyVec3 Scale;
};

//===================================================================================================================
//
// Hero class
//
//===================================================================================================================

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

	MyVec3 getPos();

protected:
	virtual void dead();

protected:
	// Mesh/Appearance elements
	SkinnedMesh1::Instance* m_instance;

	// Moving elements
	MovingEntity m_movingEnt;

	float m_time_PAA_Attack_1;
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

private:
	// Assets
	FileMesh1::MeshData     m_mesh1Datas[NUM_MESH_1_DATAS];
	SkinnedMesh1::AnimData  m_anim1Datas[NUM_ANIM_1_DATAS];
	FileMesh1::MeshTextures m_meshTextures[NUM_TEXTURES_MESHES];

	// Meshes
	SkinnedMesh1 m_skinnedMeshes[NUM_SKINNED_MESHES];
	Hero*        m_heroes[MAX_NUM_HEROES_IN_GAME];
};