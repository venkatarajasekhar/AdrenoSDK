
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

	g_PawnProps[PAWN_BROWNIE].MovingSpeed = 3.2f;
	g_PawnProps[PAWN_BROWNIE].MovingRotYOffset = 90;
	g_PawnProps[PAWN_BROWNIE].MovingTurnSpeed = 500;

	g_PawnProps[PAWN_BROWNIE].Scale = MyVec3(0.03f);

	g_PawnProps[PAWN_BROWNIE].BloodbarOffset = MyVec3(0, 3, 0);

	g_PawnProps[PAWN_BROWNIE].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_PawnProps[PAWN_BROWNIE].MeshMaterial.Diffuse = MyVec4(1.0f, 0.0f, 1.0f, 1.0f);
	g_PawnProps[PAWN_BROWNIE].MeshMaterial.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_PawnProps[PAWN_BROWNIE].MeshMaterial.Shininess = 16.0f;

	g_PawnProps[PAWN_BROWNIE].Time_PAA_Attack_1 = 0.4545f;

	// Skeleton
	g_PawnProps[PAWN_SKELETON].AttackRange = 5;
	g_PawnProps[PAWN_SKELETON].ChasingRange = 10;

	g_PawnProps[PAWN_SKELETON].MovingSpeed = 3.2f;
	g_PawnProps[PAWN_SKELETON].MovingRotYOffset = 180;
	g_PawnProps[PAWN_SKELETON].MovingTurnSpeed = 500;

	g_PawnProps[PAWN_SKELETON].Scale = MyVec3(0.01f);

	g_PawnProps[PAWN_SKELETON].BloodbarOffset = MyVec3(-1.5f, 4.3f, 0);

#ifdef WIN32
	g_PawnProps[PAWN_SKELETON].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_PawnProps[PAWN_SKELETON].MeshMaterial.Diffuse = MyVec4(1.0f, 0.0f, 1.0f, 1.0f);
	g_PawnProps[PAWN_SKELETON].MeshMaterial.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_PawnProps[PAWN_SKELETON].MeshMaterial.Shininess = 16.0f;
#elif defined __ANDROID__
	g_PawnProps[PAWN_SKELETON].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_PawnProps[PAWN_SKELETON].MeshMaterial.Diffuse = MyVec4(1.0f, 0.0f, 1.0f, 1.0f);
	g_PawnProps[PAWN_SKELETON].MeshMaterial.Specular = MyVec4(0.001f, 0.001f, 0.001f, 1.0f);
	g_PawnProps[PAWN_SKELETON].MeshMaterial.Shininess = 1.0f;
#endif
	
	g_PawnProps[PAWN_SKELETON].Time_PAA_Attack_1 = 0.392f;
}

#pragma endregion

#pragma region Constants

//=========================================================================================================
//
// Constants
//
//=========================================================================================================

static const int    PAWN_INITIAL_MAX_HEALTH = 300;
static const int    PAWN_INITIAL_DAMAGE = 20;
static const MyVec2 PAWN_BLOOD_BAR_SCALE = MyVec2(0.7f, 0.6f);
static const float  PAWN_TIME_TO_SPAWN = 35.0f;

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
	BloodBar& bloodBar,
	Quad3D& selectedDecal,
	std::vector<LivingEntity*>& lEnts,
	Audio lAudios[],
	PawnProps& pawnProp,
	TEAM_TYPE team)
{
	m_time_PAA_Attack_1 = pawnProp.Time_PAA_Attack_1;

	// Mesh/Appearance elements
	m_instance = SkinnedMesh1::buildSkinnedMeshInstance(MyVec3(), MyVec3(), pawnProp.Scale, "idle");
	m_instance->Visible = false;
	mesh.addInstance(m_instance);

	// Moving elements
	m_movingEnt.init(MyVec3(), MyVec3(), MyVec3(), pawnProp.MovingRotYOffset, pawnProp.MovingSpeed, pawnProp.MovingTurnSpeed);

	// States manager
	m_stateMachine = new StateMachine<Pawn>(this);

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
		pawnProp.AttackRange,
		&selectedDecal);

	m_audios = lAudios;
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

void Pawn::beginMatch()
{

}

void Pawn::respawn(const std::vector<MyVec3>& path)
{
	// Moving elements
	m_movingEnt.setPath(path);

	m_instance->Visible = true;
	m_inUse = true;
	m_health = PAWN_INITIAL_MAX_HEALTH;

	// States manager
	m_stateMachine->SetCurrentState(PawnState_Idle::instance());
}

MyVec3 Pawn::getPos()
{
	return m_movingEnt.getPos();
}

int Pawn::getGoldLost()
{
	return 50;
}

int Pawn::getExpLost()
{
	return 100;
}

void Pawn::dead()
{
	if (m_teamType == TEAM_TYPE_MY_TEAM) m_audios[AUDIO_MYPAWN_DEATH].play();
	if (m_teamType == TEAM_TYPE_ENEMY) m_audios[AUDIO_ENEMYPAWN_DEATH].play();
	m_instance->Visible = false;
	LivingEntity::dead();
}

void Pawn::turnToTarget()
{
	m_movingEnt.turnTo(m_atkTarget->getPos());
}

//=========================================================================================================
//
// PawnPool class
//
//=========================================================================================================

PawnPool::PawnPool()
	: m_spawnTime(0.0f)
{
}

PawnPool::~PawnPool()
{
}

void PawnPool::init(
	Shader& skinnedShader, 
	BloodBar& myBloodBar, 
	BloodBar& enemyBloodBar, 
	Quad3D& selectedDecal,
	std::vector<LivingEntity*>& lEnts,
	Audio lAudios[])
{
	initPawnProps();

	// Assets mesh data
	m_mesh1Datas[MESH_1_DATA_BROWNIE].init(resolveAssetsPath("Meshes/Pawns/brownie/Brownie5.model"));
	m_mesh1Datas[MESH_1_DATA_SKELETON].init(resolveAssetsPath("Meshes/Pawns/skeleton/Skeleton.model"));
	//m_mesh1Datas[MESH_1_DATA_SKELETON].init(resolveAssetsPath("Meshes/Pawns/catapult/Catapult.model"));

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
			//SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/catapult/CatapultFire.anim"), "attack"),
			//SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/catapult/CatapultRun.anim"), "idle"),
			//SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/catapult/CatapultRun.anim"), "run"),
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
		//resource.LoadFromFile(resolveAssetsPath("Meshes/Pawns/catapult/Catapult.pak").c_str());
		m_meshTextures[TEXTURES_MESH_SKELETON].init(m_mesh1Datas[MESH_1_DATA_SKELETON], resource);
	}

	// Skinned meshes
	m_skinnedMeshes[SKINNED_MESH_BROWNIE].init(
		m_mesh1Datas[MESH_1_DATA_BROWNIE], 
		m_anim1Datas[ANIM_1_DATA_BROWNIE], 
		m_meshTextures[TEXTURES_MESH_BROWNIE], 
		skinnedShader, 
		&g_PawnProps[PAWN_BROWNIE].MeshMaterial,
		1.5f);
	m_skinnedMeshes[SKINNED_MESH_SKELETON].init(
		m_mesh1Datas[MESH_1_DATA_SKELETON], 
		m_anim1Datas[ANIM_1_DATA_SKELETON], 
		m_meshTextures[TEXTURES_MESH_SKELETON], 
		skinnedShader, 
		&g_PawnProps[PAWN_SKELETON].MeshMaterial);

	// Pawns
	for (size_t i = 0; i < MAX_NUM_PAWNS_EACH_SIDE; i++)
	{
		m_myPawns[i].init(
			m_skinnedMeshes[SKINNED_MESH_BROWNIE], 
			myBloodBar,
			selectedDecal,
			lEnts, 
			lAudios,
			g_PawnProps[PAWN_BROWNIE], 
			TEAM_TYPE_MY_TEAM);
		m_enemyPawns[i].init(
			m_skinnedMeshes[SKINNED_MESH_SKELETON], 
			enemyBloodBar,
			selectedDecal, 
			lEnts, 
			lAudios,
			g_PawnProps[PAWN_SKELETON], 
			TEAM_TYPE_ENEMY);

		lEnts.push_back(&m_myPawns[i]);
		lEnts.push_back(&m_enemyPawns[i]);
	}

	spawnMyTeam();
	spawnEnemyTeam();
}

void PawnPool::update(Timer& timer)
{
	m_spawnTime += timer.getElapsedTime();
	if (m_spawnTime >= PAWN_TIME_TO_SPAWN)
	{
		spawnMyTeam();
		spawnEnemyTeam();

		m_spawnTime -= PAWN_TIME_TO_SPAWN;
	}

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

Pawn* PawnPool::getFreeSlot(Pawn* container, int size)
{
	for (int i = 0; i < size; i++)
	{
		Pawn* pawn = container + i;
		if (!pawn->inUse())
		{
			return pawn;
		}
	}

	return nullptr;
}

void PawnPool::spawnMyTeam()
{
	for (int i = 1; i <= 4; i++)
	{
		Pawn* pawn = getFreeSlot(m_myPawns, MAX_NUM_PAWNS_EACH_SIDE);
		if (pawn != nullptr)
		{
			pawn->respawn(spawnPath(MY_PAWN_PATH));
		}
	}
}

void PawnPool::spawnEnemyTeam()
{
	for (int i = 1; i <= 4; i++)
	{
		Pawn* pawn = getFreeSlot(m_enemyPawns, MAX_NUM_PAWNS_EACH_SIDE);
		if (pawn != nullptr)
		{
			pawn->respawn(spawnPath(ENEMY_PAWN_PATH));
		}
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
				((*i)->inUse()) &&
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
		if (
			(!pawn->m_atkTarget->inUse()) ||
			(distance_optimized(pawn->getPos(), pawn->m_atkTarget->getPos()) > pawn->m_chasingRange)
			)
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
	pawn->m_instance->setAction("attack", "", true, this, pawn->m_time_PAA_Attack_1, pawn);
	pawn->m_movingEnt.setTarget(pawn->getPos());
	pawn->turnToTarget();
	//if (pawn->getTeamType() == TEAM_TYPE_MY_TEAM) pawn->m_audios[pawn->AUDIO_MYPAWN_ATTACK]->play();
	//if (pawn->getTeamType() == TEAM_TYPE_ENEMY) pawn->m_audios[pawn->AUDIO_ENEMYPAWN_ATTACK]->play();
}

void PawnState_Attack::Execute(Pawn* pawn)
{
	if (pawn->m_atkTarget != nullptr)
	{
		if (!pawn->m_atkTarget->inUse())
		{
			pawn->m_movingEnt.reFollowPath();
			pawn->m_stateMachine->ChangeState(PawnState_Walk::instance());
		}
		else
		{
			if (distance_optimized(pawn->getPos(), pawn->m_atkTarget->getPos()) > pawn->m_atkRange)
			{
				pawn->m_stateMachine->ChangeState(PawnState_Chase::instance());
			}
		}
	}
}

void PawnState_Attack::Exit(Pawn* pawn)
{
}

void PawnState_Attack::OnPerformAAct(void* tag)
{
	if (tag != nullptr)
	{
		Pawn* pawn = (Pawn*)tag;
		pawn->m_atkTarget->accHealth(-pawn->m_damage);
		if (pawn->getTeamType() == TEAM_TYPE_MY_TEAM) pawn->m_audios[pawn->AUDIO_MYPAWN_ATTACK].play();
		if (pawn->getTeamType() == TEAM_TYPE_ENEMY) pawn->m_audios[pawn->AUDIO_ENEMYPAWN_ATTACK].play();
	}
}

#pragma endregion