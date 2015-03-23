
#pragma once

#include <MyScreen.h>

#include <MyPerspectiveCamera.h>
#include <MySpriteSheet.h>
#include <MySkinnedMesh1.h>
#include <MySkinnedMesh2.h>

#include "BloodBar.h"
#include "FlatTerrain.h"
#include "Player.h"
#include "Global.h"
#include "HUD.h"
#include "Projectile.h"

class PlayScreen : public Screen, public IOnPressListener
{
private:

	// Assets
	enum
	{
		SHADER_TERRAIN,
		SHADER_MESH,
		SHADER_SKINNED_MESH_1,
		SHADER_SKINNED_MESH_2,
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
		SPRITE_SHEET_FIREBALL,
		NUM_SPRITE_SHEETS,
	};

	enum
	{
		TEXTURES_MESH_SCORPION,
		TEXTURES_MESH_INDIA_TOWER_OF_VICTORY,
		TEXTURES_MESH_DUDE,
		NUM_TEXTURES_MESHES,
	};

	enum
	{
		MESH_1_DATA_SCORPION,
		MESH_1_DATA_INDIA_TOWER_OF_VICTORY,
		MESH_1_DATA_DUDE,
		NUM_MESH_1_DATAS,
	};

	enum
	{
		MESH_2_DATA_BOY,
		MESH_2_DATA_DMAN,
		NUM_MESH_2_DATAS,
	};

	enum
	{
		ANIM_1_DATA_SCORPION,
		ANIM_1_DATA_DUDE,
		NUM_ANIM_1_DATAS,
	};

	enum
	{
		ANIM_2_DATA_BOY,
		ANIM_2_DATA_DMAN,
		NUM_ANIM_2_DATAS,
	};

	// Effects
	enum
	{
		BILLBOARD_FIREBALL,
		NUM_BILLBOARDS,
	};

public:
	PlayScreen(ScreenManager* screenManager);
	~PlayScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

	void OnPress(const IOnPressListener::Data& data);

private:
	void cloneTrooper();

private:

	// Core objects
	PerspectiveCamera m_camera_main;
	
	// Assets
	Shader                  m_shaders[NUM_SHADERS];
	Texture                 m_textures[NUM_TEXTURES];
	SpriteSheet             m_spriteSheets[NUM_SPRITE_SHEETS];
	FileMesh1::MeshTextures m_meshTextures[NUM_TEXTURES_MESHES];
	Adreno::Model*          m_mesh1Datas[NUM_MESH_1_DATAS];
	CFrmMesh                m_mesh2Datas[NUM_MESH_2_DATAS];
	Adreno::Animation*      m_anim1Datas[NUM_ANIM_1_DATAS];
	FRM_ANIMATION_SET*      m_anim2Datas[NUM_ANIM_2_DATAS];

	// Effects
	Billboard               m_billboards[NUM_BILLBOARDS];

	// HUD objects
	BloodBar m_bloodbar_green;
	BloodBar m_bloodbar_red;
	HUD m_hud;
	
	// Mesh objects
	FlatTerrain m_mesh_terrain;

	FileMesh1 m_mesh_indiaTowerOfVictory;
	SkinnedMesh1 m_skinnedMesh_scorpion;
	SkinnedMesh1 m_skinnedMesh_dude;

	Player m_player;
	//TrooperManager m_dmanManager;
	//TrooperManager m_scorpionManager;

	// Effects objects
	Projectile m_projectile;

	float m_countTime = 2;
	bool m_lockedUserInput;
};