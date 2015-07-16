
#include "IFV.h"

#pragma region IFV properties

//===================================================================================================================
//
// IFV properties
//
//===================================================================================================================

enum
{
	IFV_CATAPULT,
	IFV_TANK,
	NUM_IFVS,
};

static IFVProps g_IFVProps[NUM_IFVS];

static void initIFVProps()
{
	// Catapult
	g_IFVProps[IFV_CATAPULT].AttackRange = 15;
	g_IFVProps[IFV_CATAPULT].ChasingRange = 10;

	g_IFVProps[IFV_CATAPULT].MovingSpeed = 2.5f;
	g_IFVProps[IFV_CATAPULT].MovingRotYOffset = 0;
	g_IFVProps[IFV_CATAPULT].MovingTurnSpeed = 300;

	g_IFVProps[IFV_CATAPULT].Scale = MyVec3(0.01f);

	g_IFVProps[IFV_CATAPULT].BloodbarOffset = MyVec3(0, 3, 0);

	g_IFVProps[IFV_CATAPULT].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_IFVProps[IFV_CATAPULT].MeshMaterial.Diffuse = MyVec4(1.0f, 0.0f, 1.0f, 1.0f);
	g_IFVProps[IFV_CATAPULT].MeshMaterial.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_IFVProps[IFV_CATAPULT].MeshMaterial.Shininess = 16.0f;

	g_IFVProps[IFV_CATAPULT].Time_PAA_Attack_1 = 0.38f;

	// Tank
	g_IFVProps[IFV_TANK].AttackRange = 15;
	g_IFVProps[IFV_TANK].ChasingRange = 10;

	g_IFVProps[IFV_TANK].MovingSpeed = 2.5f;
	g_IFVProps[IFV_TANK].MovingRotYOffset = 80;
	g_IFVProps[IFV_TANK].MovingTurnSpeed = 300;

	g_IFVProps[IFV_TANK].Scale = MyVec3(0.004f);

	g_IFVProps[IFV_TANK].BloodbarOffset = MyVec3(-1.5f, 4.3f, 0);

#ifdef WIN32
	g_IFVProps[IFV_TANK].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_IFVProps[IFV_TANK].MeshMaterial.Diffuse = MyVec4(1.0f, 0.0f, 1.0f, 1.0f);
	g_IFVProps[IFV_TANK].MeshMaterial.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_IFVProps[IFV_TANK].MeshMaterial.Shininess = 16.0f;
#elif defined __ANDROID__
	g_IFVProps[IFV_TANK].MeshMaterial.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_IFVProps[IFV_TANK].MeshMaterial.Diffuse = MyVec4(0.0f, 1.0f, 1.0f, 1.0f);
	g_IFVProps[IFV_TANK].MeshMaterial.Specular = MyVec4(0.0f, 0.0f, 0.0f, 1.0f);
	g_IFVProps[IFV_TANK].MeshMaterial.Shininess = 16.0f;
#endif
	
	g_IFVProps[IFV_TANK].Time_PAA_Attack_1 = 0.15f;
}

#pragma endregion

#pragma region Constants

//=========================================================================================================
//
// Constants
//
//=========================================================================================================

static const int    IFV_INITIAL_MAX_HEALTH = 600;
static const int    IFV_INITIAL_DAMAGE = 40;
static const MyVec2 IFV_BLOOD_BAR_SCALE = MyVec2(0.7f, 0.6f);
static const float  IFV_TIME_TO_SPAWN = 140.0f;

static const std::vector<MyVec3> MY_IFV_PATH =
{
	MyVec3(-35.3303f, 0, -1.98404f),
	MyVec3(-28.4367f, 0, -6.21993f),
	MyVec3(-19.6944f, 0, 1.42722f),
	MyVec3(-7.63194f, 0, 2.77652f),
	MyVec3(4.66686f, 0, -2.11865f),
	MyVec3(20.6128f, 0, -2.67075f),
	MyVec3(33.1632f, 0, 1.181f),
};

static const std::vector<MyVec3> ENEMY_IFV_PATH =
{
	MyVec3(30.6013f, 0, 2.89223f),
	MyVec3(14.6958f, 0, -3.19085f),
	MyVec3(0.451345f, 0, -1.58377f),
	MyVec3(-12.2037f, 0, 3.04689f),
	MyVec3(-23.3116f, 0, -4.71635f),
	MyVec3(-30.7036f, 0, -4.28849f),
	MyVec3(-37.9492f, 0, -0.463198f),
};

std::vector<MyVec3> spawnIFVPath(const std::vector<MyVec3>& basePath)
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
// IFV class
//
//=========================================================================================================

IFV::IFV()
	: m_instance(nullptr),
	m_stateMachine(nullptr),
	m_chasingRange(0)
{
}

IFV::~IFV()
{
	SAFE_DELETE(m_stateMachine);
}

void IFV::init(
	SkinnedMesh1& mesh,
	BloodBar& bloodBar,
	Quad3D& selectedDecal,
	Billboard& projtBillboard,
	ProjectilePool& projectilePool,
	std::vector<LivingEntity*>& lEnts,
	Audio lAudios[],
	IFVProps& pawnProp,
	TEAM_TYPE team)
{
	m_projtBillboard = &projtBillboard;
	m_projectilePool = &projectilePool;

	m_time_PAA_Attack_1 = pawnProp.Time_PAA_Attack_1;

	// Mesh/Appearance elements
	m_instance = SkinnedMesh1::buildSkinnedMeshInstance(MyVec3(), MyVec3(), pawnProp.Scale, "idle");
	m_instance->Visible = false;
	mesh.addInstance(m_instance);

	// Moving elements
	m_movingEnt.init(MyVec3(), MyVec3(), MyVec3(), pawnProp.MovingRotYOffset, pawnProp.MovingSpeed, pawnProp.MovingTurnSpeed);

	// States manager
	m_stateMachine = new StateMachine<IFV>(this);

	m_chasingRange = pawnProp.ChasingRange;

	setTeamType(team);
	setEntityType(ENTITY_TYPE_PAWN);

	LivingEntity::init(
		IFV_INITIAL_MAX_HEALTH,
		IFV_INITIAL_DAMAGE,
		bloodBar,
		IFV_BLOOD_BAR_SCALE,
		pawnProp.BloodbarOffset,
		lEnts,
		pawnProp.AttackRange,
		&selectedDecal);

	m_audios = lAudios;
}

void IFV::update(Timer& timer)
{
	// Moving elements
	m_movingEnt.update(timer);

	m_instance->Position = m_movingEnt.getPos();
	m_instance->Rotation = m_movingEnt.getRot();

	// States manager
	m_stateMachine->Update();
}

void IFV::beginMatch()
{

}

void IFV::respawn(const std::vector<MyVec3>& path)
{
	// Moving elements
	m_movingEnt.setPath(path);

	m_instance->Visible = true;
	m_inUse = true;
	m_health = IFV_INITIAL_MAX_HEALTH;

	// States manager
	m_stateMachine->SetCurrentState(IFVState_Idle::instance());
}

MyVec3 IFV::getPos()
{
	return m_movingEnt.getPos();
}

int IFV::getGoldLost()
{
	return 100;
}

int IFV::getExpLost()
{
	return 200;
}

void IFV::dead()
{
	m_audios[AUDIO_IFV_DEATH].play();
	m_instance->Visible = false;
	LivingEntity::dead();
}

void IFV::turnToTarget()
{
	m_movingEnt.turnTo(m_atkTarget->getPos());
}

//=========================================================================================================
//
// IFVPool class
//
//=========================================================================================================

IFVPool::IFVPool()
	: m_spawnTime(0.0f)
{
}

IFVPool::~IFVPool()
{
}

void IFVPool::init(
	Shader& skinnedShader,
	BloodBar& myBloodBar,
	BloodBar& enemyBloodBar,
	Quad3D& selectedDecal,
	Billboard& projtBillboard,
	ProjectilePool& projectilePool,
	std::vector<LivingEntity*>& lEnts,
	Audio lAudios[])
{
	initIFVProps();

	// Assets mesh data
	m_mesh1Datas[MESH_1_DATA_CATAPULT].init(resolveAssetsPath("Meshes/Pawns/catapult/Catapult.model"));
	m_mesh1Datas[MESH_1_DATA_TANK].init(resolveAssetsPath("Meshes/Pawns/tank/Tank.model"));

	// Assets anim data
	{
		const int numAnimFiles = 3;
		SkinnedMesh1::AnimFile animFiles[numAnimFiles] =
		{
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/catapult/CatapultFire.anim"), "attack"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/catapult/CatapultRun.anim"), "idle"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/catapult/CatapultRun.anim"), "run"),
		};
		m_anim1Datas[ANIM_1_DATA_CATAPULT].init(animFiles, numAnimFiles);
	}

	{
		const int numAnimFiles = 3;
		SkinnedMesh1::AnimFile animFiles[numAnimFiles] =
		{
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/tank/TankFire.anim"), "attack"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/tank/TankIdle.anim"), "idle"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Pawns/tank/TankRun.anim"), "run"),
		};
		m_anim1Datas[ANIM_1_DATA_TANK].init(animFiles, numAnimFiles);
	}

	// Assets mesh texture
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Meshes/Pawns/catapult/Catapult.pak").c_str());
		m_meshTextures[TEXTURES_MESH_CATAPULT].init(m_mesh1Datas[MESH_1_DATA_CATAPULT], resource);
	}
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Meshes/Pawns/tank/Tank.pak").c_str());
		m_meshTextures[TEXTURES_MESH_TANK].init(m_mesh1Datas[MESH_1_DATA_TANK], resource);
	}

	// Skinned meshes
	m_skinnedMeshes[SKINNED_MESH_CATAPULT].init(
		m_mesh1Datas[MESH_1_DATA_CATAPULT],
		m_anim1Datas[ANIM_1_DATA_CATAPULT],
		m_meshTextures[TEXTURES_MESH_CATAPULT],
		skinnedShader,
		&g_IFVProps[IFV_CATAPULT].MeshMaterial,
		0.5f);
	m_skinnedMeshes[SKINNED_MESH_TANK].init(
		m_mesh1Datas[MESH_1_DATA_TANK],
		m_anim1Datas[ANIM_1_DATA_TANK],
		m_meshTextures[TEXTURES_MESH_TANK],
		skinnedShader,
		&g_IFVProps[IFV_TANK].MeshMaterial,
		1.0f);

	// IFVs
	for (size_t i = 0; i < MAX_NUM_IFVS_EACH_SIDE; i++)
	{
		m_myIFVs[i].init(
			m_skinnedMeshes[SKINNED_MESH_TANK],
			myBloodBar,
			selectedDecal,
			projtBillboard,
			projectilePool,
			lEnts,
			lAudios,
			g_IFVProps[IFV_TANK],
			TEAM_TYPE_MY_TEAM);
		m_enemyIFVs[i].init(
			m_skinnedMeshes[SKINNED_MESH_CATAPULT],
			enemyBloodBar,
			selectedDecal,
			projtBillboard,
			projectilePool,
			lEnts,
			lAudios,
			g_IFVProps[IFV_CATAPULT],
			TEAM_TYPE_ENEMY);

		lEnts.push_back(&m_myIFVs[i]);
		lEnts.push_back(&m_enemyIFVs[i]);
	}

	spawnMyTeam();
	spawnEnemyTeam();
}

void IFVPool::update(Timer& timer)
{
	m_spawnTime += timer.getElapsedTime();
	if (m_spawnTime >= IFV_TIME_TO_SPAWN)
	{
		spawnMyTeam();
		spawnEnemyTeam();

		m_spawnTime -= IFV_TIME_TO_SPAWN;
	}

	for (int i = 0; i < NUM_SKINNED_MESHES; i++)
	{
		m_skinnedMeshes[i].update(timer);
	}
}

void IFVPool::render(Camera& camera, Light& light)
{
	for (int i = 0; i < NUM_SKINNED_MESHES; i++)
	{
		m_skinnedMeshes[i].render(camera, &light);
	}
}

IFV* IFVPool::getFreeSlot(IFV* container, int size)
{
	for (int i = 0; i < size; i++)
	{
		IFV* iFV = container + i;
		if (!iFV->inUse())
		{
			return iFV;
		}
	}

	return nullptr;
}

void IFVPool::spawnMyTeam()
{
	for (int i = 1; i <= 1; i++)
	{
		IFV* iFV = getFreeSlot(m_myIFVs, MAX_NUM_IFVS_EACH_SIDE);
		if (iFV != nullptr)
		{
			iFV->respawn(spawnIFVPath(MY_IFV_PATH));
		}
	}
}

void IFVPool::spawnEnemyTeam()
{
	for (int i = 1; i <= 1; i++)
	{
		IFV* iFV = getFreeSlot(m_enemyIFVs, MAX_NUM_IFVS_EACH_SIDE);
		if (iFV != nullptr)
		{
			iFV->respawn(spawnIFVPath(ENEMY_IFV_PATH));
		}
	}
}

#pragma region Pawn states

//===================================================================================================================
//
// Pawn state idle
//
//===================================================================================================================

void IFVState_Idle::Enter(IFV* iFV)
{
	iFV->m_instance->setAction("idle");
}

void IFVState_Idle::Execute(IFV* iFV)
{
	if (iFV->m_movingEnt.isMoving())
	{
		iFV->m_stateMachine->ChangeState(IFVState_Walk::instance());
	}
}

void IFVState_Idle::Exit(IFV* iFV)
{
}

//===================================================================================================================
//
// Pawn state walk
//
//===================================================================================================================

void IFVState_Walk::Enter(IFV* iFV)
{
	iFV->m_instance->setAction("run");
}

void IFVState_Walk::Execute(IFV* iFV)
{
	if (!iFV->m_movingEnt.isMoving())
	{
		iFV->m_stateMachine->ChangeState(IFVState_Idle::instance());
	}
	else
	{
		for (auto i = iFV->m_lEnts->begin(); i != iFV->m_lEnts->end(); ++i)
		{
			if ((iFV != (*i)) &&
				((*i)->inUse()) &&
				(iFV->getTeamType() != (*i)->getTeamType()) &&
				(distance_optimized(iFV->getPos(), (*i)->getPos()) <= iFV->m_chasingRange))
			{
				iFV->m_atkTarget = (*i);
				iFV->m_stateMachine->ChangeState(IFVState_Chase::instance());
				break;
			}
		}
	}
}

void IFVState_Walk::Exit(IFV* iFV)
{
}

//===================================================================================================================
//
// Pawn state chase
//
//===================================================================================================================

void IFVState_Chase::Enter(IFV* iFV)
{
	if (iFV->m_instance->CurrentAction != "run")
	{
		iFV->m_instance->setAction("run");
	}
	iFV->m_movingEnt.disFollowPath();
}

void IFVState_Chase::Execute(IFV* iFV)
{
	if (iFV->m_atkTarget != nullptr)
	{
		if (
			(!iFV->m_atkTarget->inUse()) ||
			(distance_optimized(iFV->getPos(), iFV->m_atkTarget->getPos()) > iFV->m_chasingRange)
			)
		{
			iFV->m_movingEnt.reFollowPath();
			iFV->m_stateMachine->ChangeState(IFVState_Walk::instance());
		}
		else
		{
			if (distance_optimized(iFV->getPos(), iFV->m_atkTarget->getPos()) <= iFV->m_atkRange)
			{
				iFV->m_stateMachine->ChangeState(IFVState_Attack::instance());
			}
			else
			{
				iFV->m_movingEnt.setTarget(iFV->m_atkTarget->getPos());
			}
		}
	}
}

void IFVState_Chase::Exit(IFV* iFV)
{
}

//===================================================================================================================
//
// Pawn state attack
//
//===================================================================================================================

void IFVState_Attack::Enter(IFV* iFV)
{
	iFV->m_instance->setAction("attack", "", true, this, iFV->m_time_PAA_Attack_1, iFV);
	iFV->m_movingEnt.setTarget(iFV->getPos());
	iFV->turnToTarget();
}

void IFVState_Attack::Execute(IFV* iFV)
{
	if (iFV->m_atkTarget != nullptr)
	{
		if (!iFV->m_atkTarget->inUse())
		{
			iFV->m_movingEnt.reFollowPath();
			iFV->m_stateMachine->ChangeState(IFVState_Walk::instance());
		}
		else
		{
			if (distance_optimized(iFV->getPos(), iFV->m_atkTarget->getPos()) > iFV->m_atkRange)
			{
				iFV->m_stateMachine->ChangeState(IFVState_Chase::instance());
			}
		}
	}
}

void IFVState_Attack::Exit(IFV* iFV)
{
}


void IFVState_Attack::OnPerformAAct(void* tag)
{
	if (tag != nullptr)
	{
		IFV* iFV = (IFV*)tag;
		MyVec3 offSet;

		if (iFV->getTeamType() == TEAM_TYPE_MY_TEAM) offSet = MyVec3(0, -3.7, 0);
		if (iFV->getTeamType() == TEAM_TYPE_ENEMY) offSet = MyVec3(0, -3, 0);
		iFV->m_projectilePool->spawnProjectile(
			*iFV->m_projtBillboard,
			iFV,
			iFV->m_atkTarget,
			offSet);

		if (iFV->getTeamType() == TEAM_TYPE_MY_TEAM) iFV->m_audios[iFV->AUDIO_MYIFV_ATTACK].play();
		if (iFV->getTeamType() == TEAM_TYPE_ENEMY) iFV->m_audios[iFV->AUDIO_ENEMYIFV_ATTACK].play();
	}
}

#pragma endregion