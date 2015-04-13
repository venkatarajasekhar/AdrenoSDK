
// Always including Utils.h on top
#include "Utils.h"

#include "PlayScreen.h"

//========================================================================================================
//
// Constants
//
//========================================================================================================

static const MyVec3 MAP_CENTER = MyVec3(0);
static const MyVec2 MAP_SIZE   = MyVec2(100);

//========================================================================================================
//
// PlayScreen class
//
//========================================================================================================

PlayScreen::PlayScreen(ScreenManager* screenManager)
	: Screen(screenManager),
	m_lockedUserInput(false)
{
}

PlayScreen::~PlayScreen()
{
}

void PlayScreen::init()
{
	
	/*
	// Assets sprite sheets
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/sprite_sheet.pak").c_str());

		m_spriteSheets[SPRITE_SHEET_ENERGYBALL].init(resource.GetTexture("energy_ball"), 5, MyIVec2(3, 1), MyIVec2(100, 100));
		m_spriteSheets[SPRITE_SHEET_FIREBALL].init(resource.GetTexture("fire_ball"), 5, MyIVec2(1, 1), MyIVec2(96, 32));
	}
	/**/

	/*
	// HUD objects
	m_bloodbar_green.init(m_textures[TEXTURE_BLOODBAR_GREEN_FORE], m_textures[TEXTURE_BLOODBAR_GREEN_BACK]);
	m_bloodbar_red.init(m_textures[TEXTURE_BLOODBAR_RED_FORE], m_textures[TEXTURE_BLOODBAR_RED_BACK]);
	/**/
	
	/*
	// Mesh objects
	{
		FlatTerrainProperties properties =
		{
			MyVec3(6.0f, 32.0f, 1.0f),
			MyVec2(0.0f, 1.0f),
		};

		m_mesh_terrain.init(
			m_shaders[SHADER_TERRAIN],
			m_textures[TEXTURE_TERRAIN_DIFF_1],
			m_textures[TEXTURE_TERRAIN_DIFF_2],
			m_textures[TEXTURE_TERRAIN_BLEND],
			MyVec3(0),
			MyVec2(100),
			properties);
	}
	/**/

	/*
	{
		m_scorpion.init(
			m_mesh1Datas[MESH_1_DATA_SCORPION],
			m_anim1Datas[ANIM_1_DATA_SCORPION],
			m_meshTextures[TEXTURES_MESH_SCORPION],
			m_shaders[SHADER_SKINNED_MESH_1],
			MyVec3(5, 0, 6), MyVec3(0, 45, 0), MyVec3(0.2f),
			&m_bloodbar_red,
			&m_bloodbar_green,
			m_billboards[BILLBOARD_FIREBALL],
			1000, 20, 3.0f, MY_TEAM);

		g_livingEntityManager.insertLivingEntityToList(&m_scorpion, -1);
	}
	/**/

	/*{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Boy03.pak").c_str());

		m_player.init(m_mesh2Datas[MESH_2_DATA_BOY],
			m_anim2Datas[ANIM_2_DATA_BOY],
			resource,
			m_shaders[SHADER_SKINNED_MESH_2], MyVec3(0), MyVec3(0), MyVec3(1), &m_bloodbar_green);
	}*/
	
	/*
	{
		Material material;

		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 0.5f, 0.5f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		std::map<MyString, SkinnedMesh1::AnimAction> animationPose =
		{
			{ "Stand", { 85, 65 } }, // Stand
			{ "Run", { 10, 25 } }, // Run
			{ "Beat1", { 35, 60 } }, // Beat1
			{ "Beat2", { 150, 55 } }, // Beat2
			{ "Defence", { 195, 50 } }, // Defence
			{ "Dead", { 245, 0 } }, // Dead
		};

		m_skinnedMesh_scorpion.init(
			m_mesh1Datas[MESH_1_DATA_SCORPION2],
			m_anim1Datas[ANIM_1_DATA_SCORPION2],
			m_meshTextures[TEXTURES_MESH_SCORPION2].Textures,
			m_shaders[SHADER_SKINNED_MESH_1],
			&material, &animationPose);

		g_livingEntityManager.init(&m_skinnedMesh_scorpion);
	}

	g_projectileManager.init();

	createBuilding();
	/**/

	{
		Layer_HUD::InitBundle bundle;
		bundle.MapCenter = MAP_CENTER;
		bundle.MapSize = MAP_SIZE;

		m_layer_HUD.init(bundle);
		m_layer_HUD.addPressListener(this);
	}

	{
		Layer_World::InitBundle bundle;
		bundle.MapCenter = MAP_CENTER;
		bundle.MapSize = MAP_SIZE;

		m_layer_World.init(bundle);
	}
}

/*
void PlayScreen::createBuilding()
{
	Tower* tower11 = new Tower;
	tower11->init(m_mesh1Datas[MESH_1_DATA_INDIA_TOWER_OF_VICTORY],
		m_meshTextures[TEXTURES_MESH_INDIA_TOWER_OF_VICTORY],
		m_shaders[SHADER_MESH], 
		MyVec3(-15, 0, 0), MyVec3(0), MyVec3(0.25f), &m_bloodbar_red, &m_bloodbar_green,
		m_billboards[BILLBOARD_ENERGYBALL], 1000, 40, 10, MY_TEAM);
	g_livingEntityManager.insertLivingEntityToList(tower11, -1);

	Tower* tower12 = new Tower;
	tower12->init(m_mesh1Datas[MESH_1_DATA_INDIA_TOWER_OF_VICTORY],
		m_meshTextures[TEXTURES_MESH_INDIA_TOWER_OF_VICTORY],
		m_shaders[SHADER_MESH], 
		MyVec3(-40, 0, 0), MyVec3(0), MyVec3(0.25f), &m_bloodbar_red, &m_bloodbar_green,
		m_billboards[BILLBOARD_ENERGYBALL], 1000, 40, 10, MY_TEAM);
	g_livingEntityManager.insertLivingEntityToList(tower12, -1);

	Tower* tower21 = new Tower;
	tower21->init(m_mesh1Datas[MESH_1_DATA_INDIA_TOWER_OF_VICTORY],
		m_meshTextures[TEXTURES_MESH_INDIA_TOWER_OF_VICTORY],
		m_shaders[SHADER_MESH], 
		MyVec3(15, 0, 0), MyVec3(0), MyVec3(0.25f), &m_bloodbar_red, &m_bloodbar_green,
		m_billboards[BILLBOARD_ENERGYBALL], 1000, 40, 10, ENEMY);
	g_livingEntityManager.insertLivingEntityToList(tower21, -1);

	Tower* tower22 = new Tower;
	tower22->init(m_mesh1Datas[MESH_1_DATA_INDIA_TOWER_OF_VICTORY],
		m_meshTextures[TEXTURES_MESH_INDIA_TOWER_OF_VICTORY],
		m_shaders[SHADER_MESH], 
		MyVec3(40, 0, 0), MyVec3(0), MyVec3(0.25f), &m_bloodbar_red, &m_bloodbar_green,
		m_billboards[BILLBOARD_ENERGYBALL], 1000, 40, 10, ENEMY);
	g_livingEntityManager.insertLivingEntityToList(tower22, -1);
}


void PlayScreen::cloneTrooper()
{
	Trooper* scorpion11 = new Trooper;
	scorpion11->init(TROOPER_SCORPION, MY_TEAM, 100, 10, 2.0f, MyVec3(-40, 0, 1.5), MyVec3(0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion11, TROOPER_SCORPION);

	Trooper* scorpion12 = new Trooper;
	scorpion12->init(TROOPER_SCORPION, MY_TEAM, 100, 10, 2.0f, MyVec3(-39, 0, 0), MyVec3(0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion12, TROOPER_SCORPION);

	Trooper* scorpion13 = new Trooper;
	scorpion13->init(TROOPER_SCORPION, MY_TEAM, 100, 10, 2.0f, MyVec3(-40, 0, -1.5), MyVec3(0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion13, TROOPER_SCORPION);

	Trooper* scorpion14 = new Trooper;
	scorpion14->init(TROOPER_SCORPION, MY_TEAM, 100, 10, 2.0f, MyVec3(-41, 0, 0), MyVec3(0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion14, TROOPER_SCORPION);

	Trooper* scorpion21 = new Trooper;
	scorpion21->init(TROOPER_SCORPION, ENEMY, 100, 10, 2.0f, MyVec3(40, 0, -1.5), MyVec3(0, 180, 0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion21, TROOPER_SCORPION);

	Trooper* scorpion22 = new Trooper;
	scorpion22->init(TROOPER_SCORPION, ENEMY, 100, 10, 2.0f, MyVec3(39, 0, 0), MyVec3(0, 180, 0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion22, TROOPER_SCORPION);
	
	Trooper* scorpion23 = new Trooper;
	scorpion23->init(TROOPER_SCORPION, ENEMY, 100, 10, 2.0f, MyVec3(40, 0, 1.5), MyVec3(0, 180, 0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion23, TROOPER_SCORPION);

	Trooper* scorpion24 = new Trooper;
	scorpion24->init(TROOPER_SCORPION, ENEMY, 100, 10, 2.0f, MyVec3(41, 0, 0), MyVec3(0, 180, 0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion24, TROOPER_SCORPION);
}
/**/

void PlayScreen::resize(int width, int height)
{
	m_layer_HUD.resize(width, height);
	m_layer_World.resize(width, height);
}

void PlayScreen::update(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	m_layer_HUD.update(*globalUtilObjs->timer, *globalUtilObjs->userInput);

	if (m_lockedUserInput)
	{
		globalUtilObjs->userInput->lock();
	}

	m_layer_World.update(*globalUtilObjs->timer, *globalUtilObjs->userInput);

	globalUtilObjs->userInput->unlock();
	m_lockedUserInput = false;

	/*
	m_lockedUserInput = false;

	int width, height;
	getWindowDimension(width, height);

	m_countTime += globalUtilObjs->timer->getElapsedTime();
	if (m_countTime > 30)
	{
		cloneTrooper();
		m_countTime -= 30;
	}

	if (!m_lockedUserInput)
	{
		g_livingEntityManager.update(*globalUtilObjs->userInput, *globalUtilObjs->timer, m_camera_main, width, height);
		g_projectileManager.update(*globalUtilObjs->userInput, *globalUtilObjs->timer, m_camera_main, width, height);
	}
	/**/
}

void PlayScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	m_layer_World.render(*globalUtilObjs->spriteBatch);

	{
		Layer_HUD::RenderBundle bundle;
		bundle.PlayerPos = MyVec3(0);

		m_layer_HUD.render(*globalUtilObjs->spriteBatch, bundle);
	}


	// Mesh objects

	//m_mesh_terrain.render(m_camera_main);

	/*
	{
		Light light;
		light.PosOrDir = MyVec4(0, -1, -1, 0);

		g_livingEntityManager.render(m_camera_main, light, *globalUtilObjs->spriteBatch);
		g_projectileManager.render(m_camera_main, light, *globalUtilObjs->spriteBatch);
	}
	/**/
}

void PlayScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "hud")
	{
		m_lockedUserInput = true;
	}
}