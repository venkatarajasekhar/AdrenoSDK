
#pragma once

#include <MyFileMesh1.h>
#include "LivingEntity.h"

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
// Tower class
//
//=========================================================================================================

class Tower : public LivingEntity
{
public:
	Tower();
	~Tower();

	void init(FileMesh1& mesh, const MyVec3& pos, const MyVec3& rot, const MyVec3& scale,
		BloodBar& bloodBar, const MyVec3& bloodBarOffset);
	void update(Timer& timer);

	MyVec3 getPos();

private:
	Mesh::Instance* m_instance;
};

//=========================================================================================================
//
// TowerPool class
//
//=========================================================================================================

class TowerPool
{
public:
	static const int MAX_NUM_TOWER = 6;

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
		FILE_MESH_MY_TOWER,
		FILE_MESH_ENEMY_TOWER,
		FILE_MESH_MY_MAIN_TOWER,
		FILE_MESH_ENEMY_MAIN_TOWER,
		NUM_FILE_MESHES,
	};

public:
	TowerPool();
	~TowerPool();

	void init(Shader& meshShader, BloodBar& myBloodBar, BloodBar& enemyBloodBar, std::vector<LivingEntity*>& lEnts);
	void update(Timer& timer);
	void render(Camera& camera, Light& light);

private:
	// Assets
	FileMesh1::MeshData     m_mesh1Datas[NUM_MESH_1_DATAS];
	FileMesh1::MeshTextures m_meshTextures[NUM_TEXTURES_MESHES];

	// Meshes
	FileMesh1 m_fileMeshes[NUM_FILE_MESHES];
	Tower     m_towers[MAX_NUM_TOWER];
};