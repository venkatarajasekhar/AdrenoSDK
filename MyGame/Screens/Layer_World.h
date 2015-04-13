
#pragma once

#include <MySpriteBatch.h>
#include <MySpriteSheet.h>
#include <MySkinnedMesh1.h>
#include "FlatTerrain.h"
#include "IRenderableEntity.h"
#include "LivingEntity.h"
#include "TPSCamera.h"
#include "Tower.h"
#include "Pawn.h"

class Layer_World
{
public:
	struct InitBundle
	{
		MyVec3 MapCenter;
		MyVec2 MapSize;
	};

private:
	// Assets
	enum
	{
		SHADER_TERRAIN,
		SHADER_MESH,
		SHADER_SKINNED_MESH_1,
		SHADER_BILLBOARD,
		NUM_SHADERS,
	};

	enum
	{
		TEXTURE_TERRAIN_DIFF_1,
		TEXTURE_TERRAIN_DIFF_2,
		TEXTURE_TERRAIN_BLEND,
		TEXTURE_BLOODBAR_GREEN_FORE,
		TEXTURE_BLOODBAR_GREEN_BACK,
		TEXTURE_BLOODBAR_RED_FORE,
		TEXTURE_BLOODBAR_RED_BACK,
		NUM_TEXTURES,
	};

	enum
	{
		SPRITE_SHEET_DUMP,
		NUM_SPRITE_SHEETS,
	};

	enum
	{
		MESH_1_DATA_DUMP,
		NUM_MESH_1_DATAS,
	};

	enum
	{
		ANIM_1_DATA_DUMP,
		NUM_ANIM_1_DATAS,
	};

	enum
	{
		TEXTURES_MESH_DUMP,
		NUM_TEXTURES_MESHES,
	};

public:
	Layer_World();
	~Layer_World();

	// Core functions

	void init(Layer_World::InitBundle& bundle);
	void resize(int width, int height);
	void update(Timer& timer, UserInput& userInput);
	void render(SpriteBatch& spriteBatch);

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

	// Game objects
	std::vector<IRenderableEntity*> m_renderableEnts;
	std::vector<LivingEntity*> m_livingEnts;

	TowerPool m_towerPool;
	PawnPool m_pawnPool;
};