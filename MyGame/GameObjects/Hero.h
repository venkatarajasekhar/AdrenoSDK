
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
	virtual void init(SkinnedMesh1& mesh, const MyVec3& pos, const MyVec3& rot, const MyVec3& scale,
		BloodBar& bloodBar, const MyVec3& bloodBarOffset);
	virtual void update(Timer& timer);

	StateMachine<Hero>* getFSM()const;
	MyVec3 getPos();

	// Controlling hero
	void changeAnimAction(const MyString& action);
	bool isMoving();

protected:
	// Mesh/Appearance elements
	SkinnedMesh1::Instance* m_instance;

	// Moving elements
	MovingEntity m_movingEnt;

	// States manager
	StateMachine<Hero>* m_stateMachine;
};

//===================================================================================================================
//
// HeroPool class
//
//===================================================================================================================

class HeroPool
{
public:
	static const int MAX_NUM_HEROES = 2;

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
		SKINNED_MESH_MY_HERO_1,
		SKINNED_MESH_ENEMY_HERO_1,
		NUM_SKINNED_MESHES,
	};

public:
	HeroPool();
	~HeroPool();

	void init(Shader& skinnedShader, BloodBar& myBloodBar, BloodBar& enemyBloodBar, std::vector<LivingEntity*>& lEnts, OnPressListenee& map);
	void update(Timer& timer);
	void render(Camera& camera, Light& light);

private:
	// Assets
	FileMesh1::MeshData     m_mesh1Datas[NUM_MESH_1_DATAS];
	SkinnedMesh1::AnimData  m_anim1Datas[NUM_ANIM_1_DATAS];
	FileMesh1::MeshTextures m_meshTextures[NUM_TEXTURES_MESHES];

	// Meshes
	SkinnedMesh1 m_skinnedMeshes[NUM_SKINNED_MESHES];
	Hero*        m_heroes[MAX_NUM_HEROES];
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