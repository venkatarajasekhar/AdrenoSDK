
#pragma once

#include <MyFileMesh1.h>
#include "LivingEntity.h"
#include "EventListener.h"
#include "StateMachine.h"
#include "Projectile.h"
#include <MyAudio.h>

#pragma region Struct

//=========================================================================================================
//
// Struct
//
//=========================================================================================================

struct TowerProps
{
	int InitialMaxHealth;
	int InitialDamage;

	float AttackRange;

	MyVec2 BloodbarScale;
	MyVec3 BloodbarOffset;

	Material MeshMaterial;
};

struct TowerInGameProp
{
	MyVec3 Pos;
	MyVec3 Rot;
	MyVec3 Scale;
};

#pragma endregion

//=========================================================================================================
//
// Tower class
//
//=========================================================================================================

class Tower : public LivingEntity
{
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

	Tower();
	virtual ~Tower();

	virtual void init(
		FileMesh1& mesh,
		BloodBar& bloodBar,
		Quad3D& selectedDecal,
		Billboard& projtBillboard,
		ProjectilePool& projectilePool,
		std::vector<LivingEntity*>& lEnts,
		Audio lAudios[],
		TowerProps& towerProp,
		TowerInGameProp& towerInGameProp,
		TEAM_TYPE team);
	virtual void update(Timer& timer);

	MyVec3 getPos();

protected:
	virtual void dead();

protected:
	Mesh::Instance* m_instance;

	// States manager
	StateMachine<Tower>* m_stateMachine;

	float m_timeElapsed;

	Billboard* m_projtBillboard;
	ProjectilePool* m_projectilePool;

	Audio* m_audios;

protected:
	friend class TowerState_Idle;
	friend class TowerState_Attack;
};

//=========================================================================================================
//
// TowerPool class
//
//=========================================================================================================

class TowerPool
{
public:
	enum
	{
		TOWER_IN_GAME_MY_MAIN_TOWER,
		TOWER_IN_GAME_MY_TOWER_1,
		TOWER_IN_GAME_MY_TOWER_2,
		TOWER_IN_GAME_ENEMY_MAIN_TOWER,
		TOWER_IN_GAME_ENEMY_TOWER_1,
		TOWER_IN_GAME_ENEMY_TOWER_2,
		MAX_NUM_TOWER_IN_GAME,
	};

private:
	// Assets
	enum
	{
		MESH_1_DATA_HOUSE_WIND,
		MESH_1_DATA_OUTPOST,
		MESH_1_DATA_TOWER_OF_VICTORY,
		MESH_1_DATA_WHITE_PAGODA,
		NUM_MESH_1_DATAS,
	};

	enum
	{
		TEXTURES_MESH_HOUSE_WIND,
		TEXTURES_MESH_OUTPOST,
		TEXTURES_MESH_TOWER_OF_VICTORY,
		TEXTURES_MESH_WHITE_PAGODA,
		NUM_TEXTURES_MESHES,
	};

	// Meshes
	enum
	{
		FILE_MESH_HOUSE_WIND,
		FILE_MESH_OUTPOST,
		FILE_MESH_TOWER_OF_VICTORY,
		FILE_MESH_WHITE_PAGODA,
		NUM_FILE_MESHES,
	};

public:
	TowerPool();
	~TowerPool();

	void init(
		Shader& meshShader, 
		BloodBar& myBloodBar, 
		BloodBar& enemyBloodBar, 
		Quad3D& selectedDecal,
		Billboard& projtBillboard,
		ProjectilePool& projectilePool,
		std::vector<LivingEntity*>& lEnts,
		Audio lAudios[],
		IOnGameOverListener* gameOverListener);
	void update(Timer& timer);
	void render(Camera& camera, Light& light);

private:
	// Assets
	FileMesh1::MeshData     m_mesh1Datas[NUM_MESH_1_DATAS];
	FileMesh1::MeshTextures m_meshTextures[NUM_TEXTURES_MESHES];

	// Meshes
	FileMesh1 m_fileMeshes[NUM_FILE_MESHES];
	Tower*    m_towers[MAX_NUM_TOWER_IN_GAME];
};

#pragma region Tower state

//=========================================================================================================
//
// Tower state
//
//=========================================================================================================

class TowerState_Idle : public State<Tower>
{
private:
	TowerState_Idle(){}
	TowerState_Idle(const TowerState_Idle&);
	TowerState_Idle& operator=(const TowerState_Idle&);

public:
	static TowerState_Idle* instance(){ static TowerState_Idle ins; return &ins; }

public:
	virtual void Enter(Tower* tower);
	virtual void Execute(Tower* tower);
	virtual void Exit(Tower* tower);
};

class TowerState_Attack : public State<Tower>
{
private:
	TowerState_Attack(){}
	TowerState_Attack(const TowerState_Attack&);
	TowerState_Attack& operator=(const TowerState_Attack&);

public:
	static TowerState_Attack* instance(){ static TowerState_Attack ins; return &ins; }

public:
	virtual void Enter(Tower* tower);
	virtual void Execute(Tower* tower);
	virtual void Exit(Tower* tower);
};

#pragma endregion