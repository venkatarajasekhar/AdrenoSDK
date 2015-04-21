
#pragma once

#include <MyFileMesh1.h>
#include "LivingEntity.h"
#include "EventListener.h"
#include "StateMachine.h"
#include "Projectile.h"

/*
#include <MySkinnedMesh1.h>
#include <MySpriteSheet.h>
#include <MyInput.h>
#include "Global.h"
#include "Projectile.h"


class Tower : public LivingEntity
{
private:
	FileMesh1 m_tower;
	FileMesh1::Instance* m_instance;
	BloodBar* m_bloodBar;
	Billboard m_billboard;
	int m_idEmemy = -1;
	float m_countTime = 3;
public:

	void init(
		Adreno::Model* model,
		FileMesh1::MeshTextures& textures,
		Shader& shader,
		const MyVec3& pos,
		const MyVec3& rot,
		const MyVec3& scale,
		BloodBar* bloodBarRed,
		BloodBar* bloodBarGreen,
		Billboard& billboard,
		int health,
		int damage,
		float range,
		TEAM_TYPE teamType);

	void update(UserInput& userInput, Timer& timer, Camera& camera, int width, int height);
	void render(Camera& camera, Light& light, SpriteBatch& spriteBatch);
	int findLivingEntityToBeat();
	FileMesh1::Instance* getInstance();
};
/**/

//----------------------------------------------------------------------------------------------------------------------------------------

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

//=========================================================================================================
//
// Tower class
//
//=========================================================================================================

class Tower : public LivingEntity
{
public:
	Tower();
	virtual ~Tower();

	virtual void init(
		FileMesh1& mesh,
		BloodBar& bloodBar,
		Billboard& projtBillboard,
		ProjectilePool& projectilePool,
		std::vector<LivingEntity*>& lEnts,
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
		Billboard& projtBillboard,
		ProjectilePool& projectilePool,
		std::vector<LivingEntity*>& lEnts,
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