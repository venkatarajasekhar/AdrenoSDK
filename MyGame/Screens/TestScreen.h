
#pragma once

#include <MyScreen.h>

#include <MyPerspectiveCamera.h>
#include <MySpriteSheet.h>
#include <MySkinnedMesh1.h>
#include <MySkinnedMesh2.h>
#include <MyEventListener.h>
#include <MyFont.h>

#include "HUD.h"
#include "FlatTerrain.h"
#include "IRenderableEntity.h"

class TestScreen : public Screen, public IOnPressListener
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
		TEXTURES_MESH_MOUSER_BOSS,
		NUM_TEXTURES_MESHES,
	};

	enum
	{
		MESH_1_DATA_SCORPION,
		MESH_1_DATA_INDIA_TOWER_OF_VICTORY,
		MESH_1_DATA_DUDE,
		MESH_1_DATA_MOUSER_BOSS,
		NUM_MESH_1_DATAS,
	};

	enum
	{
		ANIM_1_DATA_SCORPION,
		ANIM_1_DATA_DUDE,
		ANIM_1_DATA_MOUSER_BOSS,
		NUM_ANIM_1_DATAS,
	};

	enum
	{
		FONT_CONSOLAS_12,
		NUM_FONTS,
	};

	// Effects
	enum
	{
		BILLBOARD_FIREBALL,
		NUM_BILLBOARDS,
	};

public:
	TestScreen(ScreenManager* screenManager);
	~TestScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

	void OnPress(const IOnPressListener::Data& data);

private:
	void initAssets();
	void initHeroes();

private:

	// Core objects
	PerspectiveCamera m_camera_main;
	
	// Assets
	Shader                  m_shaders[NUM_SHADERS];
	Texture                 m_textures[NUM_TEXTURES];
	SpriteSheet             m_spriteSheets[NUM_SPRITE_SHEETS];
	FileMesh1::MeshTextures m_meshTextures[NUM_TEXTURES_MESHES];
	Adreno::Model*          m_mesh1Datas[NUM_MESH_1_DATAS];
	Adreno::Animation*      m_anim1Datas[NUM_ANIM_1_DATAS];
	Font                    m_fonts[NUM_FONTS];

	// Effects

	// HUD objects
	HUD m_hud;
	
	// Mesh objects
	FlatTerrain m_mesh_terrain;

	// Game objects
	std::vector<IRenderableEntity*> m_renderableEnts;

	// Misc
	bool m_lockedUserInput;
};