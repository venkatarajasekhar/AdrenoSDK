
// Always including Utils.h on top
#include "Utils.h"

#include "TestScreen.h"
#include "Hero_Controlled.h"

Hero_Controlled* g_heroPlayer = nullptr;

//========================================================================================================
//
// Parameters
//
//========================================================================================================

static const MyVec3 MAIN_CAM_POS = MyVec3(0.0f, 10.0f, 10.0f);
static const MyVec3 MAIN_CAM_TARGET = MyVec3(0.0f, 0.0f, 0.0f);

static const float MAIN_CAM_FAR = 100.0f;

//========================================================================================================
//
// PlayScreen class
//
//========================================================================================================

TestScreen::TestScreen(ScreenManager* screenManager)
	: Screen(screenManager),
	m_lockedUserInput(false)
{
}

TestScreen::~TestScreen()
{
	// Game objects
	for (auto i = m_renderableEnts.begin(); i != m_renderableEnts.end(); ++i)
	{
		delete (*i);
	}

	// Assets mesh 1 datas
	for (size_t i = 0; i < NUM_MESH_1_DATAS; i++)
	{
		Adreno::FrmDestroyLoadedModel(m_mesh1Datas[i]);
	}

	// Assets anim 1 datas
	for (size_t i = 0; i < NUM_ANIM_1_DATAS; i++)
	{
		Adreno::FrmDestroyLoadedAnimation(m_anim1Datas[i]);
	}
}

#pragma region Init assets

void TestScreen::initAssets()
{
	// Assets shaders
	m_shaders[SHADER_TERRAIN].init(
		resolveAssetsPath("Shaders/terrain.vs"),
		resolveAssetsPath("Shaders/terrain.fs"),
		PosTexVertex::ShaderAttribsDesc,
		PosTexVertex::NumShaderAttribsDesc);

	m_shaders[SHADER_MESH].init(
		resolveAssetsPath("Shaders/mesh.vs"),
		resolveAssetsPath("Shaders/PhongShading.fs"),
		PosNorTexVertex::ShaderAttribsDesc,
		PosNorTexVertex::NumShaderAttribsDesc);

	m_shaders[SHADER_SKINNED_MESH_1].init(
		resolveAssetsPath("Shaders/skinnedMesh1.vs"),
		resolveAssetsPath("Shaders/PhongShading.fs"),
		SkinnedVertex::ShaderAttribsDesc,
		SkinnedVertex::NumShaderAttribsDesc);

	m_shaders[SHADER_SKINNED_MESH_2].init(
		resolveAssetsPath("Shaders/skinnedMesh2.vs"),
		resolveAssetsPath("Shaders/PhongShading.fs"),
		SkinnedVertex::ShaderAttribsDesc,
		SkinnedVertex::NumShaderAttribsDesc);

	m_shaders[SHADER_BILLBOARD].init(
		resolveAssetsPath("Shaders/billboard.vs"),
		resolveAssetsPath("Shaders/billboard.fs"),
		PosTexVertex::ShaderAttribsDesc,
		PosTexVertex::NumShaderAttribsDesc);

	// Assets textures
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/terrain.pak").c_str());

		m_textures[TEXTURE_TERRAIN_DIFF_1].init(resource.GetTexture("diffuse_1"));
		m_textures[TEXTURE_TERRAIN_DIFF_2].init(resource.GetTexture("diffuse_2"));
		m_textures[TEXTURE_TERRAIN_BLEND].init(resource.GetTexture("blend"));
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/gui_play.pak").c_str());

		m_textures[TEXTURE_BLOODBAR_GREEN_FORE].init(resource.GetTexture("bloodbar_foreground"));
		m_textures[TEXTURE_BLOODBAR_GREEN_BACK].init(resource.GetTexture("bloodbar_background"));
		m_textures[TEXTURE_BLOODBAR_RED_FORE].init(resource.GetTexture("bloodbar_enemy_foreground"));
		m_textures[TEXTURE_BLOODBAR_RED_BACK].init(resource.GetTexture("bloodbar_enemy_background"));
	}

	// Assets sprite sheets
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/sprite_sheet.pak").c_str());

		m_spriteSheets[SPRITE_SHEET_FIRE_BALL].init(resource.GetTexture("fire_ball"), 10, MyIVec2(3, 1), MyIVec2(32, 32));
		m_spriteSheets[SPRITE_SHEET_ENERGY_BALL].init(resource.GetTexture("energy_ball"), 5, MyIVec2(3, 1), MyIVec2(100, 100));
	}

	// Assets mesh 1 datas
	m_mesh1Datas[MESH_1_DATA_SCORPION] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/scorpion.model").c_str());
	m_mesh1Datas[MESH_1_DATA_INDIA_TOWER_OF_VICTORY] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/india_tower_of_victory.model").c_str());
	m_mesh1Datas[MESH_1_DATA_DUDE] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/dude.model").c_str());
	m_mesh1Datas[MESH_1_DATA_MOUSER_BOSS] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/Heroes/MouserBoss/mouser_boss.model").c_str());

	// Assets anim 1 datas
	m_anim1Datas[ANIM_1_DATA_SCORPION] = Adreno::FrmLoadAnimationFromFile(resolveAssetsPath("Meshes/scorpion.anim").c_str());
	m_anim1Datas[ANIM_1_DATA_DUDE] = Adreno::FrmLoadAnimationFromFile(resolveAssetsPath("Meshes/dude.anim").c_str());

	// Assets mesh textures
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Scorpion.pak").c_str());

		m_meshTextures[TEXTURES_MESH_SCORPION].init(m_mesh1Datas[MESH_1_DATA_SCORPION], resource);
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/india_tower_of_victory.pak").c_str());

		m_meshTextures[TEXTURES_MESH_INDIA_TOWER_OF_VICTORY].init(m_mesh1Datas[MESH_1_DATA_INDIA_TOWER_OF_VICTORY], resource);
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/dude.pak").c_str());

		m_meshTextures[TEXTURES_MESH_DUDE].init(m_mesh1Datas[MESH_1_DATA_DUDE], resource);
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Meshes/Heroes/MouserBoss/mouser_boss.pak").c_str());

		m_meshTextures[TEXTURES_MESH_MOUSER_BOSS].init(m_mesh1Datas[MESH_1_DATA_MOUSER_BOSS], resource);
	}

	// Assets fonts
	m_fonts[FONT_CONSOLAS_12].init(resolveAssetsPath("Fonts/Consolas12.pak"));
}

#pragma endregion

#pragma region Init heroes

void TestScreen::initHeroes()
{
	/*
	{
		Hero_Controlled* hero = new Hero_Controlled;

		Material material;

		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 0.5f, 0.5f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		std::map<MyString, SkinnedMesh1::AnimAction> actions =
		{
			{ "Idle",         { 85 , 65 } },
			{ "Walk",         { 10 , 25 } },
			{ "Attack",       { 35 , 60 } },
			{ "Skill_Attack", { 150, 55 } },
			{ "Defence",      { 195, 50 } },
			{ "Die",          { 245, 0  } },
		};

		hero->init(
			m_mesh1Datas[MESH_1_DATA_SCORPION],
			m_anim1Datas[ANIM_1_DATA_SCORPION],
			m_meshTextures[TEXTURES_MESH_SCORPION].Textures,
			m_shaders[SHADER_SKINNED_MESH_1],
			material,
			MyVec3(),
			MyVec3(),
			MyVec3(0.2f),
			actions);

		m_renderableEnts.push_back(hero);

		m_mesh_terrain.addPressListener(hero);

		g_heroPlayer = hero;
	}
	/**/

	/**/
	{
		Hero_Controlled* hero = new Hero_Controlled;

		Material material;

		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		const int numAnimFiles = 2;
		SkinnedMesh1::AnimFile animFiles[numAnimFiles] = 
		{
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/MouserBoss/walk.anim"), "Walk"),
			SkinnedMesh1::AnimFile(resolveAssetsPath("Meshes/Heroes/MouserBoss/idle.anim"), "Idle"),
		};

		std::map<MyString, SkinnedMesh1::AnimAction> actions;

		SkinnedMesh1::mergeAnimFile(animFiles, numAnimFiles, m_anim1Datas[ANIM_1_DATA_MOUSER_BOSS], actions);

		hero->init(
			m_mesh1Datas[MESH_1_DATA_MOUSER_BOSS],
			m_anim1Datas[ANIM_1_DATA_MOUSER_BOSS],
			m_meshTextures[TEXTURES_MESH_MOUSER_BOSS].Textures,
			m_shaders[SHADER_SKINNED_MESH_1],
			material,
			MyVec3(),
			MyVec3(),
			MyVec3(0.005f),
			actions);

		m_renderableEnts.push_back(hero);

		m_mesh_terrain.addPressListener(hero);

		g_heroPlayer = hero;
	}
	/**/
}

#pragma endregion

void TestScreen::init()
{
	// Core objects
	m_camera_main.init(MAIN_CAM_POS, MAIN_CAM_TARGET, 45.0f, 0.1f, MAIN_CAM_FAR);

	initAssets();
	initHeroes();

	// HUD objects
	m_hud.init(MyVec3(0), MyVec2(100), this);
	m_hud.addPressListener(this);

	// Effects
	m_billboards[BILLBOARD_FIRE_BALL].init(&m_spriteSheets[SPRITE_SHEET_FIRE_BALL], m_shaders[SHADER_BILLBOARD], MyVec3(0, 3, 0), MyVec2(1), 0);
	m_billboards[BILLBOARD_ENERGY_BALL].init(&m_spriteSheets[SPRITE_SHEET_ENERGY_BALL], m_shaders[SHADER_BILLBOARD], MyVec3(3, 3, 3), MyVec2(1), 0);

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
}

void TestScreen::resize(int width, int height)
{
	// Core objects
	m_camera_main.resize(width, height);

	// HUD objects
	m_hud.resize(width, height);
}

void TestScreen::update(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Core objects
	{
		MyVec3 offset = MyVec3(0, 15, 15);
		MyVec3 eye = g_heroPlayer->getPos() + offset;
		m_camera_main.setEye(eye);

		m_camera_main.update();
	}

	// Assets sprite sheets
	for (int i = 0; i < NUM_SPRITE_SHEETS; i++)
	{
		m_spriteSheets[i].update(*globalUtilObjs->timer);
	}

	// Effects
	m_billboards[BILLBOARD_ENERGY_BALL].setPos(MyVec3(-5.0f * globalUtilObjs->timer->getTotalTime(), 3, 0));
	for (int i = 0; i < NUM_BILLBOARDS; i++)
	{
		m_billboards[i].update(*globalUtilObjs->timer);
	}

	// HUD objects
	m_hud.update(*globalUtilObjs->timer, *globalUtilObjs->userInput);
	if (m_lockedUserInput)
	{
		globalUtilObjs->userInput->lock();
	}

	// Mesh objects
	m_mesh_terrain.update(*globalUtilObjs->timer, *globalUtilObjs->userInput, m_camera_main);

	// Game objects
	for (auto i = m_renderableEnts.begin(); i != m_renderableEnts.end(); ++i)
	{
		(*i)->update(*globalUtilObjs->userInput, *globalUtilObjs->timer);
	}

	// Misc
	globalUtilObjs->userInput->unlock();
	m_lockedUserInput = false;
}

void TestScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Mesh objects
	m_mesh_terrain.render(m_camera_main);

	// Game objects
	Light light;
	light.PosOrDir = MyVec4(0, -1, -1, 0);

	for (auto i = m_renderableEnts.begin(); i != m_renderableEnts.end(); ++i)
	{
		(*i)->render(m_camera_main, light);
	}

	// Effects objects
	for (int i = 0; i < NUM_BILLBOARDS; i++)
	{
		m_billboards[i].render(m_camera_main);
	}
		
	// HUD objects
	m_hud.render(*globalUtilObjs->spriteBatch);
	globalUtilObjs->spriteBatch->renderText2D(
		m_fonts[FONT_CONSOLAS_12],
		toString(globalUtilObjs->timer->getFPS()),
		MyVec2(10, 10));
}

void TestScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "hud")
	{
		m_lockedUserInput = true;
	}
}