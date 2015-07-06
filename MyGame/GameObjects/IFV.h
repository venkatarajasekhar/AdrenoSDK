
#pragma once

#include <MySkinnedMesh1.h>
#include "LivingEntity.h"
#include "MovingEntity.h"
#include "StateMachine.h"
#include "Projectile.h"
#include <MyAudio.h>

#pragma region Structs

//=========================================================================================================
//
// Structs
//
//=========================================================================================================

struct IFVProps
{
	float AttackRange;
	float ChasingRange;

	float MovingSpeed;
	float MovingRotYOffset;
	float MovingTurnSpeed;

	MyVec3 Scale;

	MyVec3 BloodbarOffset;

	Material MeshMaterial;

	float Time_PAA_Attack_1;
};

#pragma endregion

//=========================================================================================================
//
// IFV class
//
//=========================================================================================================

class IFV : public LivingEntity
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
		NUM_AUDIOS,
	};

	IFV();
	~IFV();

	void init(
		SkinnedMesh1& mesh,
		BloodBar& bloodBar,
		Quad3D& selectedDecal,
		Billboard& projtBillboard,
		ProjectilePool& projectilePool,
		std::vector<LivingEntity*>& lEnts,
		Audio lAudios[],
		IFVProps& pawnProp,
		TEAM_TYPE team);
	void update(Timer& timer);

	void respawn(const std::vector<MyVec3>& path);

	MyVec3 getPos();

	void turnToTarget();

private:
	void dead();

private:
	// Mesh/Appearance elements
	SkinnedMesh1::Instance* m_instance;

	// Moving elements
	MovingEntity m_movingEnt;

	// States manager
	StateMachine<IFV>* m_stateMachine;

	float m_chasingRange;
	float m_time_PAA_Attack_1;

	Billboard* m_projtBillboard;
	ProjectilePool* m_projectilePool;

	Audio* m_audios[NUM_AUDIOS];

private:
	friend class IFVState_Idle;
	friend class IFVState_Walk;
	friend class IFVState_Chase;
	friend class IFVState_Attack;
};

//=========================================================================================================
//
// IFVPool class
//
//=========================================================================================================

class IFVPool
{
public:
	static const int MAX_NUM_IFVS_EACH_SIDE = 8;

private:
	// Assets
	enum
	{
		MESH_1_DATA_CATAPULT,
		MESH_1_DATA_TANK,
		NUM_MESH_1_DATAS,
	};

	enum
	{
		ANIM_1_DATA_CATAPULT,
		ANIM_1_DATA_TANK,
		NUM_ANIM_1_DATAS,
	};

	enum
	{
		TEXTURES_MESH_CATAPULT,
		TEXTURES_MESH_TANK,
		NUM_TEXTURES_MESHES,
	};

	// Meshes
	enum
	{
		SKINNED_MESH_CATAPULT,
		SKINNED_MESH_TANK,
		NUM_SKINNED_MESHES,
	};

public:
	IFVPool();
	~IFVPool();

	void init(
		Shader& skinnedShader,
		BloodBar& myBloodBar,
		BloodBar& enemyBloodBar,
		Quad3D& selectedDecal,
		Billboard& projtBillboard,
		ProjectilePool& projectilePool,
		std::vector<LivingEntity*>& lEnts,
		Audio lAudios[]);
	void update(Timer& timer);
	void render(Camera& camera, Light& light);

private:
	IFV* getFreeSlot(IFV* container, int size);

	void spawnMyTeam();
	void spawnEnemyTeam();

private:
	// Assets
	FileMesh1::MeshData     m_mesh1Datas[NUM_MESH_1_DATAS];
	SkinnedMesh1::AnimData  m_anim1Datas[NUM_ANIM_1_DATAS];
	FileMesh1::MeshTextures m_meshTextures[NUM_TEXTURES_MESHES];

	// Meshes
	SkinnedMesh1 m_skinnedMeshes[NUM_SKINNED_MESHES];

	IFV m_myIFVs[MAX_NUM_IFVS_EACH_SIDE];
	IFV m_enemyIFVs[MAX_NUM_IFVS_EACH_SIDE];

	float m_spawnTime;
};

#pragma region Pawn state

//=========================================================================================================
//
// IFV state
//
//=========================================================================================================

class IFVState_Idle : public State<IFV>
{
private:
	IFVState_Idle(){}
	IFVState_Idle(const IFVState_Idle&);
	IFVState_Idle& operator=(const IFVState_Idle&);

public:
	static IFVState_Idle* instance(){ static IFVState_Idle ins; return &ins; }

public:
	virtual void Enter(IFV* iFV);
	virtual void Execute(IFV* iFV);
	virtual void Exit(IFV* iFV);
};

class IFVState_Walk : public State<IFV>
{
private:
	IFVState_Walk(){}
	IFVState_Walk(const IFVState_Walk&);
	IFVState_Walk& operator=(const IFVState_Walk&);

public:
	static IFVState_Walk* instance(){ static IFVState_Walk ins; return &ins; }

public:
	virtual void Enter(IFV* iFV);
	virtual void Execute(IFV* iFV);
	virtual void Exit(IFV* iFV);
};

class IFVState_Chase : public State<IFV>
{
private:
	IFVState_Chase(){}
	IFVState_Chase(const IFVState_Chase&);
	IFVState_Chase& operator=(const IFVState_Chase&);

public:
	static IFVState_Chase* instance(){ static IFVState_Chase ins; return &ins; }

public:
	virtual void Enter(IFV* iFV);
	virtual void Execute(IFV* iFV);
	virtual void Exit(IFV* iFV);
};

class IFVState_Attack : public State<IFV>, public SkinnedMesh1::IOnPerformAActListener
{
private:
	IFVState_Attack(){}
	IFVState_Attack(const IFVState_Attack&);
	IFVState_Attack& operator=(const IFVState_Attack&);

public:
	static IFVState_Attack* instance(){ static IFVState_Attack ins; return &ins; }

public:
	virtual void Enter(IFV* iFV);
	virtual void Execute(IFV* iFV);
	virtual void Exit(IFV* iFV);

	void OnPerformAAct(void* tag);
};

#pragma endregion