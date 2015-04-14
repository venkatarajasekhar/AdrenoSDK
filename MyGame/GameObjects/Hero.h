
#pragma once

#include <MySkinnedMesh1.h>
#include <MyEventListener.h>
#include "LivingEntity.h"
#include "MovingEntity.h"
#include "StateMachine.h"

//===================================================================================================================
//
// Hero consists of state:
//	- Idle
//	- Walk
//	- Attack
//	- Skill attack
//	- Die
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
		int iHero,
		TEAM_TYPE team);
	virtual void update(Timer& timer);

	MyVec3 getPos();

protected:
	// Mesh/Appearance elements
	SkinnedMesh1::Instance* m_instance;

	// Moving elements
	MovingEntity m_movingEnt;

	// States manager
	StateMachine<Hero>* m_stateMachine;

protected:
	friend class HeroState_Idle;
	friend class HeroState_Walk;
	friend class HeroState_Attack;
};

//===================================================================================================================
//
// HeroPool class
//
//===================================================================================================================

class HeroPool
{
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

	// Heroes in-game
	enum
	{
		HERO_IN_GAME_MY_HERO_1,
		HERO_IN_GAME_ENEMY_HERO_1,
		MAX_NUM_HEROES_IN_GAME,
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

//===================================================================================================================
//
// Hero states
//
//===================================================================================================================

class HeroState_Idle : public State<Hero>
{
private:
	HeroState_Idle(){}
	HeroState_Idle(const HeroState_Idle&);
	HeroState_Idle& operator=(const HeroState_Idle&);

public:
	static HeroState_Idle* instance(){ static HeroState_Idle ins; return &ins; }

public:
	virtual void Enter(Hero* hero);
	virtual void Execute(Hero* hero);
	virtual void Exit(Hero* hero);
};

class HeroState_Walk : public State<Hero>
{
private:
	HeroState_Walk(){}
	HeroState_Walk(const HeroState_Walk&);
	HeroState_Walk& operator=(const HeroState_Walk&);

public:
	static HeroState_Walk* instance(){ static HeroState_Walk ins; return &ins; }

public:
	virtual void Enter(Hero* hero);
	virtual void Execute(Hero* hero);
	virtual void Exit(Hero* hero);
};

class HeroState_Attack : public State<Hero>
{
private:
	HeroState_Attack(){}
	HeroState_Attack(const HeroState_Attack&);
	HeroState_Attack& operator=(const HeroState_Attack&);

public:
	static HeroState_Attack* instance(){ static HeroState_Attack ins; return &ins; }

public:
	virtual void Enter(Hero* hero);
	virtual void Execute(Hero* hero);
	virtual void Exit(Hero* hero);
};