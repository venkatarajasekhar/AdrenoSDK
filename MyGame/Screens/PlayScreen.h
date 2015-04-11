
#pragma once

#include <MyScreen.h>
#include "Layer_HUD.h"
#include "Layer_World.h"

//class PlayScreen : public Screen, public IOnPressListener
class PlayScreen : public Screen
{
private:

	/*
	// Effects
	enum
	{
		BILLBOARD_FIREBALL,
		BILLBOARD_ENERGYBALL,
		NUM_BILLBOARDS,
	};
	/**/

public:
	PlayScreen(ScreenManager* screenManager);
	~PlayScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

	//void OnPress(const IOnPressListener::Data& data);

private:
	//void cloneTrooper();
	//void createBuilding();

private:
	/*
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
	SkinnedMesh1 m_skinnedMesh_dude;
	SkinnedMesh1 m_skinnedMesh_scorpion;

	//Player m_player;
	ScorpionHero m_scorpion;

	float m_countTime = 28;
	bool m_lockedUserInput;
	/**/

	Layer_HUD   m_layer_HUD;
	Layer_World m_layer_World;
};