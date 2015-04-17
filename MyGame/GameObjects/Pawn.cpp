
#include "Pawn.h"

#pragma region Pawn properties

//===================================================================================================================
//
// Pawn properties
//
//===================================================================================================================

enum
{
	PAWN_BROWNIE,
	PAWN_SKELETON,
	NUM_PAWNS,
};

static PawnProps g_PawnProps[NUM_PAWNS];

static void initPawnProps()
{
	// Brownie
	g_PawnProps[PAWN_BROWNIE].AttackRange = 5;
	g_PawnProps[PAWN_BROWNIE].ChasingRange = 10;

	g_PawnProps[PAWN_BROWNIE].MovingSpeed = 3;
	g_PawnProps[PAWN_BROWNIE].MovingRotYOffset = 90;
	g_PawnProps[PAWN_BROWNIE].MovingTurnSpeed = 500;

	g_PawnProps[PAWN_BROWNIE].Scale = MyVec3(0.03f);

	g_PawnProps[PAWN_BROWNIE].BloodbarOffset = MyVec3(0, 3, 0);

	g_PawnProps[PAWN_BROWNIE].Material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_PawnProps[PAWN_BROWNIE].Material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_PawnProps[PAWN_BROWNIE].Material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_PawnProps[PAWN_BROWNIE].Material.Shininess = 16.0f;

	g_PawnProps[PAWN_BROWNIE].Time_PAA_Attack_1 = 0.4545f;

	// Skeleton
	g_PawnProps[PAWN_SKELETON].AttackRange = 5;
	g_PawnProps[PAWN_SKELETON].ChasingRange = 10;

	g_PawnProps[PAWN_SKELETON].MovingSpeed = 3.5f;
	g_PawnProps[PAWN_SKELETON].MovingRotYOffset = 180;
	g_PawnProps[PAWN_SKELETON].MovingTurnSpeed = 500;

	g_PawnProps[PAWN_SKELETON].Scale = MyVec3(0.01f);

	g_PawnProps[PAWN_SKELETON].BloodbarOffset = MyVec3(-1.5f, 4.5f, 0);

	g_PawnProps[PAWN_SKELETON].Material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_PawnProps[PAWN_SKELETON].Material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_PawnProps[PAWN_SKELETON].Material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_PawnProps[PAWN_SKELETON].Material.Shininess = 16.0f;

	g_PawnProps[PAWN_SKELETON].Time_PAA_Attack_1 = 0.392f;
}

#pragma endregion

#pragma region Constants

//=========================================================================================================
//
// Constants
//
//=========================================================================================================

static const int    PAWN_INITIAL_MAX_HEALTH = 50;
static const int    PAWN_INITIAL_DAMAGE = 2;
static const MyVec2 PAWN_BLOOD_BAR_SCALE = MyVec2(0.7f, 0.6f);

static const std::vector<MyVec3> MY_PAWN_PATH = 
{
	MyVec3(-35.3303f, 0, -1.98404f),
	MyVec3(-28.4367f, 0, -6.21993f),
	MyVec3(-19.6944f, 0, 1.42722f),
	MyVec3(-7.63194f, 0, 2.77652f),
	MyVec3(4.66686f, 0, -2.11865f),
	MyVec3(20.6128f, 0, -2.67075f),
	MyVec3(33.1632f, 0, 1.181f),
};

static const std::vector<MyVec3> ENEMY_PAWN_PATH =
{
	MyVec3(30.6013f, 0, 2.89223f),
	MyVec3(14.6958f, 0, -3.19085f),
	MyVec3(0.451345f, 0, -1.58377f),
	MyVec3(-12.2037f, 0, 3.04689f),
	MyVec3(-23.3116f, 0, -4.71635f),
	MyVec3(-30.7036f, 0, -4.28849f),
	MyVec3(-37.9492f, 0, -0.463198f),
};

std::vector<MyVec3> spawnPath(const std::vector<MyVec3>& basePath)
{
	std::vector<MyVec3> path;
	path.reserve(basePath.size());

	for (auto i = basePath.begin(); i != basePath.end(); ++i)
	{
		MyVec3 pivot = (*i);
		pivot += MyVec3(random(-5.0f, 5.0f), 0, random(-5.0f, 5.0f));
		path.push_back(pivot);
	}

	return path;
}

#pragma endregion

//=========================================================================================================
//
// Pawn class
//
//=========================================================================================================

Pawn::Pawn()
	: m_instance(nullptr),
	m_stateMachine(nullptr),
	m_chasingRange(0)
{
}

Pawn::~Pawn()
{
	SAFE_DELETE(m_stateMachine);
}

void Pawn::init(
	SkinnedMesh1& mesh,
	const std::vector<MyVec3>& path,
	BloodBar& bloodBar,
	std::vector<LivingEntity*>& lEnts,
	PawnProps& pawnProp,
	TEAM_TYPE team)
{
	// Mesh/Appearance elements
	m_instance = SkinnedMesh1::buildSkinnedMeshInstance(MyVec3(), MyVec3(), pawnProp.Scale, "idle");
	mesh.addInstance(m_instance);

	// Moving elements
	m_movingEnt.init(path, pawnProp.MovingRotYOffset, pawnProp.MovingSpeed, pawnProp.MovingTurnSpeed);

	// States manager
	m_stateMachine = new StateMachine<Pawn>(this);
	m_stateMachine->SetCurrentState(PawnState_Idle::instance());

	m_chasingRange = pawnProp.ChasingRange;

	setTeamType(team);
	setEntityType(ENTITY_TYPE_PAWN);

	LivingEntity::init(
		PAWN_INITIAL_MAX_HEALTH, 
		PAWN_INITIAL_DAMAGE, 
		bloodBar, 
		PAWN_BLOOD_BAR_SCALE, 
		pawnProp.BloodbarOffset,
		lEnts, 
		pawnProp.AttackRange);
}

void Pawn::update(Timer& timer)
{
	// Moving elements
	m_movingEnt.update(timer);

	m_instance->Position = m_movingEnt.getPos();
	m_instance->Rotation = m_movingEnt.getRot();

	// States manager
	m_stateMachine->Update();
}

MyVec3 Pawn::getPos()
{
	return m_movingEnt.getPos();
}

//=========================================================================================================
//
// PawnPool class
//
//=========================================================================================================

PawnPool::PawnPool()
{
}

PawnPool::~PawnPool()
{
}

void PawnPool::init(Shader& skinnedShader, BloodBar& myBloodBar, BloodBar& enemyBloodBar, std::vector<LivingEntity*>& lEnts)
{
	initPawnProps();

	// Assets mesh data
	m_mesh1Datas[MESH_1_DATA_BROWNIE].init(resolveAssetsPath("Meshes/Pawns/brownie/Brownie5.model"));
	m_mesh1Datas[MESH_1_DATA_SKELETON].init(resolveAssetsPath("Meshes/Pawns/skeleton/Skeleton.model"));

	// Assets anim data
	{
		const int numAnimFiles = 3;
		SkinnedMesh1::AnimFile animFiles[numAnimFiles] =
		{
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/brownie/atk2.anim"), "attack"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/brownie/idle.anim"), "idle"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/brownie/run.anim"), "run"),
		};
		m_anim1Datas[ANIM_1_DATA_BROWNIE].init(animFiles, numAnimFiles);
	}

	{
		const int numAnimFiles = 3;
		SkinnedMesh1::AnimFile animFiles[numAnimFiles] =
		{
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/skeleton/1hmattacka.anim"), "attack"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/skeleton/1hmidle.anim"), "idle"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/skeleton/1hmfastforward.anim"), "run"),
		};
		m_anim1Datas[ANIM_1_DATA_SKELETON].init(animFiles, numAnimFiles);
	}
	
	// Assets mesh texture
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Meshes/Pawns/brownie/Brownie5.pak").c_str());
		m_meshTextures[TEXTURES_MESH_BROWNIE].init(m_mesh1Datas[MESH_1_DATA_BROWNIE], resource);
	}
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Meshes/Pawns/skeleton/Skeleton.pak").c_str());
		m_meshTextures[TEXTURES_MESH_SKELETON].init(m_mesh1Datas[MESH_1_DATA_SKELETON], resource);
	}

	// Skinned meshes
	m_skinnedMeshes[SKINNED_MESH_BROWNIE].init(
		m_mesh1Datas[MESH_1_DATA_BROWNIE], 
		m_anim1Datas[ANIM_1_DATA_BROWNIE], 
		m_meshTextures[TEXTURES_MESH_BROWNIE], 
		skinnedShader, 
		&g_PawnProps[PAWN_BROWNIE].Material,
		1.5f);
	m_skinnedMeshes[SKINNED_MESH_SKELETON].init(
		m_mesh1Datas[MESH_1_DATA_SKELETON], 
		m_anim1Datas[ANIM_1_DATA_SKELETON], 
		m_meshTextures[TEXTURES_MESH_SKELETON], 
		skinnedShader, 
		&g_PawnProps[PAWN_SKELETON].Material);

	// Pawns
	m_pawns[0].init(m_skinnedMeshes[SKINNED_MESH_BROWNIE], spawnPath(MY_PAWN_PATH), myBloodBar, lEnts, g_PawnProps[PAWN_BROWNIE], TEAM_TYPE_MY_TEAM);
	m_pawns[1].init(m_skinnedMeshes[SKINNED_MESH_BROWNIE], spawnPath(MY_PAWN_PATH), myBloodBar, lEnts, g_PawnProps[PAWN_BROWNIE], TEAM_TYPE_MY_TEAM);
	m_pawns[2].init(m_skinnedMeshes[SKINNED_MESH_BROWNIE], spawnPath(MY_PAWN_PATH), myBloodBar, lEnts, g_PawnProps[PAWN_BROWNIE], TEAM_TYPE_MY_TEAM);

	m_pawns[3].init(m_skinnedMeshes[SKINNED_MESH_SKELETON], spawnPath(ENEMY_PAWN_PATH), enemyBloodBar, lEnts, g_PawnProps[PAWN_SKELETON], TEAM_TYPE_ENEMY);
	m_pawns[4].init(m_skinnedMeshes[SKINNED_MESH_SKELETON], spawnPath(ENEMY_PAWN_PATH), enemyBloodBar, lEnts, g_PawnProps[PAWN_SKELETON], TEAM_TYPE_ENEMY);
	m_pawns[5].init(m_skinnedMeshes[SKINNED_MESH_SKELETON], spawnPath(ENEMY_PAWN_PATH), enemyBloodBar, lEnts, g_PawnProps[PAWN_SKELETON], TEAM_TYPE_ENEMY);

	// Fill into list of living entities
	for (size_t i = 0; i < MAX_NUM_PAWNS; i++)
	{
		lEnts.push_back(&m_pawns[i]);
	}
}

void PawnPool::update(Timer& timer)
{
	for (int i = 0; i < NUM_SKINNED_MESHES; i++)
	{
		m_skinnedMeshes[i].update(timer);
	}
}

void PawnPool::render(Camera& camera, Light& light)
{
	for (int i = 0; i < NUM_SKINNED_MESHES; i++)
	{
		m_skinnedMeshes[i].render(camera, &light);
	}
}

#pragma region Pawn states

//===================================================================================================================
//
// Pawn state idle
//
//===================================================================================================================

void PawnState_Idle::Enter(Pawn* pawn)
{
	pawn->m_instance->setAction("idle");
}

void PawnState_Idle::Execute(Pawn* pawn)
{
	if (pawn->m_movingEnt.isMoving())
	{
		pawn->m_stateMachine->ChangeState(PawnState_Walk::instance());
	}
}

void PawnState_Idle::Exit(Pawn* pawn)
{
}

//===================================================================================================================
//
// Pawn state walk
//
//===================================================================================================================

void PawnState_Walk::Enter(Pawn* pawn)
{
	pawn->m_instance->setAction("run");
}

void PawnState_Walk::Execute(Pawn* pawn)
{
	if (!pawn->m_movingEnt.isMoving())
	{
		pawn->m_stateMachine->ChangeState(PawnState_Idle::instance());
	}
	else
	{
		for (auto i = pawn->m_lEnts->begin(); i != pawn->m_lEnts->end(); ++i)
		{
			if ((pawn != (*i)) &&
				(pawn->getTeamType() != (*i)->getTeamType()) &&
				(distance_optimized(pawn->getPos(), (*i)->getPos()) <= pawn->m_chasingRange))
			{
				pawn->m_atkTarget = (*i);
				pawn->m_stateMachine->ChangeState(PawnState_Chase::instance());
				break;
			}
		}
	}
}

void PawnState_Walk::Exit(Pawn* pawn)
{
}

//===================================================================================================================
//
// Pawn state chase
//
//===================================================================================================================

void PawnState_Chase::Enter(Pawn* pawn)
{
	if (pawn->m_instance->CurrentAction != "run")
	{
		pawn->m_instance->setAction("run");
	}
	pawn->m_movingEnt.disFollowPath();
}

void PawnState_Chase::Execute(Pawn* pawn)
{
	if (pawn->m_atkTarget != nullptr)
	{
		if (distance_optimized(pawn->getPos(), pawn->m_atkTarget->getPos()) > pawn->m_chasingRange)
		{
			pawn->m_movingEnt.reFollowPath();
			pawn->m_stateMachine->ChangeState(PawnState_Walk::instance());
		}
		else
		{
			if (distance_optimized(pawn->getPos(), pawn->m_atkTarget->getPos()) <= pawn->m_atkRange)
			{
				pawn->m_stateMachine->ChangeState(PawnState_Attack::instance());
			}
			else
			{
				pawn->m_movingEnt.setTarget(pawn->m_atkTarget->getPos());
			}
		}
	}
}

void PawnState_Chase::Exit(Pawn* pawn)
{
}

//===================================================================================================================
//
// Pawn state attack
//
//===================================================================================================================

void PawnState_Attack::Enter(Pawn* pawn)
{
	pawn->m_instance->setAction("attack");
	pawn->m_movingEnt.setTarget(pawn->getPos());
}

void PawnState_Attack::Execute(Pawn* pawn)
{
	if (pawn->m_atkTarget != nullptr)
	{
		if (distance_optimized(pawn->getPos(), pawn->m_atkTarget->getPos()) > pawn->m_atkRange)
		{
			pawn->m_stateMachine->ChangeState(PawnState_Chase::instance());
		}
		else
		{
			pawn->m_atkTarget->accHealth(-pawn->m_damage);
		}
	}
}

void PawnState_Attack::Exit(Pawn* pawn)
{
}

#pragma endregion