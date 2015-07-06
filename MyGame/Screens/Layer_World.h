
#pragma once

#include <MySpriteBatch.h>
#include <MySpriteSheet.h>
#include <MySkinnedMesh1.h>
#include <MyBillboard.h>
#include "FlatTerrain.h"
#include "Shop.h"
#include "LivingEntity.h"
#include "TPSCamera.h"
#include "Tower.h"
#include "Pawn.h"
#include "IFV.h"
#include "Hero.h"
#include "EventListener.h"
#include <MySphere.h>
#include <MyAudio.h>

class Layer_World
{
public:
	struct InitBundle
	{
		MyVec3 MapCenter;
		MyVec2 MapSize;

		IOnGameOverListener* GameOverListener;
		IOnPressListener* ShopListener;
	};

private:
	// Assets
	enum
	{
		SHADER_TERRAIN,
		SHADER_MESH,
		SHADER_SKINNED_MESH_1,
		SHADER_BILLBOARD,
		SHADER_TEXTURED_MESH,
		SHADER_TEST_SHAPE,
		NUM_SHADERS,
	};

	enum
	{
		TEXTURE_TERRAIN_DIFF_1,
		TEXTURE_TERRAIN_DIFF_2,
		TEXTURE_TERRAIN_BLEND,
		TEXTURE_TERRAIN_DECAL_SELECTED,
		TEXTURE_BLOODBAR_GREEN_FORE,
		TEXTURE_BLOODBAR_GREEN_BACK,
		TEXTURE_BLOODBAR_RED_FORE,
		TEXTURE_BLOODBAR_RED_BACK,
		TEXTURE_SHAPE_SMOKE,
		NUM_TEXTURES,
	};

	enum
	{
		SPRITE_SHEET_ENERGY_BALL,
		SPRITE_SHEET_BULLET,
		NUM_SPRITE_SHEETS,
	};

	enum
	{
		MESH_1_DATA_SHOP,
		NUM_MESH_1_DATAS,
	};

	enum
	{
		ANIM_1_DATA_DUMP,
		NUM_ANIM_1_DATAS,
	};

	enum
	{
		TEXTURES_MESH_SHOP,
		NUM_TEXTURES_MESHES,
	};

	// Graphics objects
	enum
	{
		BILLBOARD_ENERGY_BALL,
		BILLBOARD_BULLET,
		NUM_BILLBOARDS,
	};

	enum
	{
		BLOOD_BAR_MY_TEAM,
		BLOOD_BAR_ENEMY,
		NUM_BLOOD_BARS,
	};

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
		NUM_AUDIOS,
	};

public:
	Layer_World();
	~Layer_World();

	// Core functions

	void init(Layer_World::InitBundle& bundle);
	void resize(int width, int height);
	void update(Timer& timer, UserInput& userInput);
	void render(SpriteBatch& spriteBatch);

	Hero* getPlayer();

private:
	// Assets
	Shader                  m_shaders[NUM_SHADERS];
	Texture                 m_textures[NUM_TEXTURES];
	SpriteSheet             m_spriteSheets[NUM_SPRITE_SHEETS];
	FileMesh1::MeshData     m_mesh1Datas[NUM_MESH_1_DATAS];
	SkinnedMesh1::AnimData  m_anim1Datas[NUM_ANIM_1_DATAS];
	FileMesh1::MeshTextures m_meshTextures[NUM_TEXTURES_MESHES];

	// Core objects
	TPSCamera m_camera_main;

	// Mesh objects
	FlatTerrain m_mesh_terrain;
	Shop m_shop;
	Quad3D m_selectedDecal;
	Sphere m_dumpSphere;

	// Game objects
	std::vector<LivingEntity*> m_livingEnts;
	LivingEntity* m_selectedGameObj;

	TowerPool m_towerPool;
	PawnPool m_pawnPool;
	IFVPool m_iFVPool;
	HeroPool m_heroPool;
	ProjectilePool m_projectilePool;

	// Graphics objects
	Billboard m_billboards[NUM_BILLBOARDS];
	BloodBar m_bloodBar[NUM_BLOOD_BARS];

	Audio m_audios[NUM_AUDIOS];
};