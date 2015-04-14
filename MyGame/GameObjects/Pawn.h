
#pragma once

#include <MySkinnedMesh1.h>
#include "LivingEntity.h"

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
		const MyVec3& pos, 
		const MyVec3& rot, 
		const MyVec3& scale,
		BloodBar& bloodBar, 
		std::vector<LivingEntity*>& lEnts,
		int iPawn,
		TEAM_TYPE team);
	void update(Timer& timer);

	MyVec3 getPos();

private:
	SkinnedMesh1::Instance* m_instance;
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