
#include "Pawn.h"

#pragma region Pawn properties

//===================================================================================================================
//
// Pawn properties
//
//===================================================================================================================

struct PawnProps
{
	float AttackRange;

	float MovingSpeed;
	float MovingRotYOffset;
	float MovingTurnSpeed;

	MyVec3 BloodbarOffset;

	Material Material;
};

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

	g_PawnProps[PAWN_BROWNIE].MovingSpeed = 5;
	g_PawnProps[PAWN_BROWNIE].MovingRotYOffset = 0;
	g_PawnProps[PAWN_BROWNIE].MovingTurnSpeed = 500;

	g_PawnProps[PAWN_BROWNIE].BloodbarOffset = MyVec3(0, 3, 0);

	g_PawnProps[PAWN_BROWNIE].Material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_PawnProps[PAWN_BROWNIE].Material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_PawnProps[PAWN_BROWNIE].Material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_PawnProps[PAWN_BROWNIE].Material.Shininess = 16.0f;

	// Skeleton
	g_PawnProps[PAWN_SKELETON].AttackRange = 5;

	g_PawnProps[PAWN_SKELETON].MovingSpeed = 5;
	g_PawnProps[PAWN_SKELETON].MovingRotYOffset = 0;
	g_PawnProps[PAWN_SKELETON].MovingTurnSpeed = 500;

	g_PawnProps[PAWN_SKELETON].BloodbarOffset = MyVec3(-1.5f, 4.5f, 0);

	g_PawnProps[PAWN_SKELETON].Material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	g_PawnProps[PAWN_SKELETON].Material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	g_PawnProps[PAWN_SKELETON].Material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	g_PawnProps[PAWN_SKELETON].Material.Shininess = 16.0f;
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

#pragma endregion

//=========================================================================================================
//
// Pawn class
//
//=========================================================================================================

Pawn::Pawn()
	: m_instance(nullptr)
{
}

Pawn::~Pawn()
{
}

void Pawn::init(
	SkinnedMesh1& mesh,
	const MyVec3& pos,
	const MyVec3& rot,
	const MyVec3& scale,
	BloodBar& bloodBar,
	std::vector<LivingEntity*>& lEnts,
	int iPawn)
{
	PawnProps* pawnProp = g_PawnProps + iPawn;

	m_instance = SkinnedMesh1::buildSkinnedMeshInstance(pos, rot, scale, "idle");
	mesh.addInstance(m_instance);

	LivingEntity::init(
		PAWN_INITIAL_MAX_HEALTH, 
		PAWN_INITIAL_DAMAGE, 
		bloodBar, 
		PAWN_BLOOD_BAR_SCALE, 
		pawnProp->BloodbarOffset, 
		lEnts, 
		pawnProp->AttackRange);
}

void Pawn::update(Timer& timer)
{
}

MyVec3 Pawn::getPos()
{
	return m_instance->Position;
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
		&g_PawnProps[PAWN_BROWNIE].Material);
	m_skinnedMeshes[SKINNED_MESH_SKELETON].init(
		m_mesh1Datas[MESH_1_DATA_SKELETON], 
		m_anim1Datas[ANIM_1_DATA_SKELETON], 
		m_meshTextures[TEXTURES_MESH_SKELETON], 
		skinnedShader, 
		&g_PawnProps[PAWN_SKELETON].Material);

	// Pawns
	m_pawns[0].init(m_skinnedMeshes[SKINNED_MESH_BROWNIE], MyVec3(-35.0f, 0, -8.0f), MyVec3(0), MyVec3(0.03f), myBloodBar, lEnts, PAWN_BROWNIE);
	m_pawns[1].init(m_skinnedMeshes[SKINNED_MESH_BROWNIE], MyVec3(-34.6f, 0, -3.0f), MyVec3(0), MyVec3(0.03f), myBloodBar, lEnts, PAWN_BROWNIE);
	m_pawns[2].init(m_skinnedMeshes[SKINNED_MESH_BROWNIE], MyVec3(-35.6f, 0, 2.0f), MyVec3(0), MyVec3(0.03f), myBloodBar, lEnts, PAWN_BROWNIE);

	m_pawns[3].init(m_skinnedMeshes[SKINNED_MESH_SKELETON], MyVec3(28.0f, 0, -6.0f), MyVec3(0, 90, 0), MyVec3(0.01f), enemyBloodBar, lEnts, PAWN_SKELETON);
	m_pawns[4].init(m_skinnedMeshes[SKINNED_MESH_SKELETON], MyVec3(27.4f, 0, -1.0f), MyVec3(0, 90, 0), MyVec3(0.01f), enemyBloodBar, lEnts, PAWN_SKELETON);
	m_pawns[5].init(m_skinnedMeshes[SKINNED_MESH_SKELETON], MyVec3(28.6f, 0, 4.0f), MyVec3(0, 90, 0), MyVec3(0.01f), enemyBloodBar, lEnts, PAWN_SKELETON);

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