
#include "Pawn.h"

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

void Pawn::init(SkinnedMesh1& mesh, const MyVec3& pos, const MyVec3& rot, const MyVec3& scale)
{
	m_instance = SkinnedMesh1::buildSkinnedMeshInstance(pos, rot, scale, "idle");
	mesh.addInstance(m_instance);
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

void PawnPool::init(Shader& skinnedShader)
{
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

	Material material;

	material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
	material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
	material.Shininess = 16.0f;

	m_skinnedMeshes[SKINNED_MESH_MY_PAWN].init(m_mesh1Datas[MESH_1_DATA_BROWNIE], m_anim1Datas[ANIM_1_DATA_BROWNIE], m_meshTextures[TEXTURES_MESH_BROWNIE], skinnedShader, &material);
	m_skinnedMeshes[SKINNED_MESH_ENEMY_PAWN].init(m_mesh1Datas[MESH_1_DATA_SKELETON], m_anim1Datas[ANIM_1_DATA_SKELETON], m_meshTextures[TEXTURES_MESH_SKELETON], skinnedShader, &material);

	// Pawns
	m_pawns[0].init(m_skinnedMeshes[SKINNED_MESH_MY_PAWN], MyVec3(-35.0f, 0, -8.0f), MyVec3(0), MyVec3(0.03f));
	m_pawns[1].init(m_skinnedMeshes[SKINNED_MESH_MY_PAWN], MyVec3(-34.6f, 0, -3.0f), MyVec3(0), MyVec3(0.03f));
	m_pawns[2].init(m_skinnedMeshes[SKINNED_MESH_MY_PAWN], MyVec3(-35.6f, 0, 2.0f), MyVec3(0), MyVec3(0.03f));

	m_pawns[3].init(m_skinnedMeshes[SKINNED_MESH_ENEMY_PAWN], MyVec3(28.0f, 0, -6.0f), MyVec3(0, 90, 0), MyVec3(0.01f));
	m_pawns[4].init(m_skinnedMeshes[SKINNED_MESH_ENEMY_PAWN], MyVec3(27.4f, 0, -1.0f), MyVec3(0, 90, 0), MyVec3(0.01f));
	m_pawns[5].init(m_skinnedMeshes[SKINNED_MESH_ENEMY_PAWN], MyVec3(28.6f, 0, 4.0f), MyVec3(0, 90, 0), MyVec3(0.01f));
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