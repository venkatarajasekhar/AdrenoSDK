
#pragma once

#include <MySkinnedMesh1.h>
#include "LivingEntity.h"
#include "MovingEntity.h"
#include "StateMachine.h"

//=========================================================================================================
//
// Structs
//
//=========================================================================================================

struct PawnProps
{
	float AttackRange;
	float ChasingRange;

	float MovingSpeed;
	float MovingRotYOffset;
	float MovingTurnSpeed;

	MyVec3 Scale;

	MyVec3 BloodbarOffset;

	Material Material;

	float Time_PAA_Attack_1;
};

//=========================================================================================================
//
// Pawn class
//
//=========================================================================================================

class Pawn : public LivingEntity
{
public:
	Pawn();
	~Pawn();

	void init(
		SkinnedMesh1& mesh, 
		const std::vector<MyVec3>& path,
		BloodBar& bloodBar, 
		std::vector<LivingEntity*>& lEnts,
		PawnProps& pawnProp,
		TEAM_TYPE team);
	void update(Timer& timer);

	MyVec3 getPos();

private:
	void dead();

private:
	// Mesh/Appearance elements
	SkinnedMesh1::Instance* m_instance;

	// Moving elements
	MovingEntity m_movingEnt;

	// States manager
	StateMachine<Pawn>* m_stateMachine;

	float m_chasingRange;
	float m_time_PAA_Attack_1;

private:
	friend class PawnState_Idle;
	friend class PawnState_Walk;
	friend class PawnState_Chase;
	friend class PawnState_Attack;
};

//=========================================================================================================
//
// PawnPool class
//
//=========================================================================================================

class PawnPool
{
public:
	static const int MAX_NUM_PAWNS = 6;

private:
	// Assets
	enum
	{
		MESH_1_DATA_BROWNIE,
		MESH_1_DATA_SKELETON,
		NUM_MESH_1_DATAS,
	};

	enum
	{
		ANIM_1_DATA_BROWNIE,
		ANIM_1_DATA_SKELETON,
		NUM_ANIM_1_DATAS,
	};

	enum
	{
		TEXTURES_MESH_BROWNIE,
		TEXTURES_MESH_SKELETON,
		NUM_TEXTURES_MESHES,
	};

	// Meshes
	enum
	{
		SKINNED_MESH_BROWNIE,
		SKINNED_MESH_SKELETON,
		NUM_SKINNED_MESHES,
	};

public:
	PawnPool();
	~PawnPool();

	void init(Shader& skinnedShader, BloodBar& myBloodBar, BloodBar& enemyBloodBar, std::vector<LivingEntity*>& lEnts);
	void update(Timer& timer);
	void render(Camera& camera, Light& light);

private:
	// Assets
	FileMesh1::MeshData     m_mesh1Datas[NUM_MESH_1_DATAS];
	SkinnedMesh1::AnimData  m_anim1Datas[NUM_ANIM_1_DATAS];
	FileMesh1::MeshTextures m_meshTextures[NUM_TEXTURES_MESHES];

	// Meshes
	SkinnedMesh1 m_skinnedMeshes[NUM_SKINNED_MESHES];
	Pawn         m_pawns[MAX_NUM_PAWNS];
};

//=========================================================================================================
//
// Pawn state
//
//=========================================================================================================

class PawnState_Idle : public State<Pawn>
{
private:
	PawnState_Idle(){}
	PawnState_Idle(const PawnState_Idle&);
	PawnState_Idle& operator=(const PawnState_Idle&);

public:
	static PawnState_Idle* instance(){ static PawnState_Idle ins; return &ins; }

public:
	virtual void Enter(Pawn* pawn);
	virtual void Execute(Pawn* pawn);
	virtual void Exit(Pawn* pawn);
};

class PawnState_Walk : public State<Pawn>
{
private:
	PawnState_Walk(){}
	PawnState_Walk(const PawnState_Walk&);
	PawnState_Walk& operator=(const PawnState_Walk&);

public:
	static PawnState_Walk* instance(){ static PawnState_Walk ins; return &ins; }

public:
	virtual void Enter(Pawn* pawn);
	virtual void Execute(Pawn* pawn);
	virtual void Exit(Pawn* pawn);
};

class PawnState_Chase : public State<Pawn>
{
private:
	PawnState_Chase(){}
	PawnState_Chase(const PawnState_Chase&);
	PawnState_Chase& operator=(const PawnState_Chase&);

public:
	static PawnState_Chase* instance(){ static PawnState_Chase ins; return &ins; }

public:
	virtual void Enter(Pawn* pawn);
	virtual void Execute(Pawn* pawn);
	virtual void Exit(Pawn* pawn);
};

class PawnState_Attack : public State<Pawn>, public SkinnedMesh1::IOnPerformAActListener
{
private:
	PawnState_Attack(){}
	PawnState_Attack(const PawnState_Attack&);
	PawnState_Attack& operator=(const PawnState_Attack&);

public:
	static PawnState_Attack* instance(){ static PawnState_Attack ins; return &ins; }

public:
	virtual void Enter(Pawn* pawn);
	virtual void Execute(Pawn* pawn);
	virtual void Exit(Pawn* pawn);

	void OnPerformAAct(void* tag);
};