
// Always including Utils.h on top
#include "Utils.h"

#include "PlayScreen.h"

//========================================================================================================
//
// Parameters
//
//========================================================================================================

static const MyVec3 MAIN_CAM_POS = MyVec3(0.0f, 10.0f, 10.0f);
static const MyVec3 MAIN_CAM_TARGET = MyVec3(0.0f, 0.0f, 0.0f);

static const float MAIN_CAM_FAR = 100.0f;

extern MyVec3 PositionPlayer;
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

	// Assets anim 2 datas
	for (size_t i = 0; i < NUM_ANIM_2_DATAS; i++)
	{
		delete m_anim2Datas[i];
	}
}

void PlayScreen::init()
{
	// Core objects
	m_camera_main.init(MAIN_CAM_POS, MAIN_CAM_TARGET, 45.0f, 0.1f, MAIN_CAM_FAR);

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
		resource.LoadFromFile(resolveAssetsPath("Textures/Terrain.pak").c_str());

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
		resource.LoadFromFile(resolveAssetsPath("Textures/sprite_sheets.pak").c_str());

		m_spriteSheets[SPRITE_SHEET_FIREBALL].init(resource.GetTexture("fireball"), 10, MyIVec2(3, 2), MyIVec2(128, 128));
	}

	// Assets mesh 1 datas
	m_mesh1Datas[MESH_1_DATA_SCORPION] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/scorpion.model").c_str());
	m_mesh1Datas[MESH_1_DATA_SCORPION2] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/scorpion.model").c_str());
	m_mesh1Datas[MESH_1_DATA_INDIA_TOWER_OF_VICTORY] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/india_tower_of_victory.model").c_str());
	m_mesh1Datas[MESH_1_DATA_DUDE] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/dude.model").c_str());

	// Assets mesh 2 datas
	m_mesh2Datas[MESH_2_DATA_BOY].Load(resolveAssetsPath("Meshes/Boy03.mesh").c_str());
	m_mesh2Datas[MESH_2_DATA_DMAN].Load(resolveAssetsPath("Meshes/Dman.mesh").c_str());

	// Assets anim 1 datas
	m_anim1Datas[ANIM_1_DATA_SCORPION] = Adreno::FrmLoadAnimationFromFile(resolveAssetsPath("Meshes/scorpion.anim").c_str());
	m_anim1Datas[ANIM_1_DATA_SCORPION2] = Adreno::FrmLoadAnimationFromFile(resolveAssetsPath("Meshes/scorpion.anim").c_str());
	m_anim1Datas[ANIM_1_DATA_DUDE] = Adreno::FrmLoadAnimationFromFile(resolveAssetsPath("Meshes/dude.anim").c_str());

	// Assets anim 2 datas
	FrmReadAnimation(resolveAssetsPath("Meshes/Boy03.anim").c_str(), &m_anim2Datas[ANIM_2_DATA_BOY]);
	FrmReadAnimation(resolveAssetsPath("Meshes/Dman.anim").c_str(), &m_anim2Datas[ANIM_2_DATA_DMAN]);

	// Assets mesh textures
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Scorpion.pak").c_str());

		m_meshTextures[TEXTURES_MESH_SCORPION].init(m_mesh1Datas[MESH_1_DATA_SCORPION], resource);
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Scorpion.pak").c_str());

		m_meshTextures[TEXTURES_MESH_SCORPION2].init(m_mesh1Datas[MESH_1_DATA_SCORPION2], resource);
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

	// Effects
	m_billboards[BILLBOARD_FIREBALL].init(&m_spriteSheets[SPRITE_SHEET_FIREBALL], m_shaders[SHADER_BILLBOARD], MyVec3(0), MyVec2(2), 0);
	
	// HUD objects
	m_bloodbar_green.init(m_textures[TEXTURE_BLOODBAR_GREEN_FORE], m_textures[TEXTURE_BLOODBAR_GREEN_BACK]);
	m_bloodbar_red.init(m_textures[TEXTURE_BLOODBAR_RED_FORE], m_textures[TEXTURE_BLOODBAR_RED_BACK]);
	{
		m_hud.init(MyVec3(0), MyVec2(100));
		m_hud.addPressListener(this);
	}
	
	// Mesh objects
	{
		FlatTerrainProperties properties =
		{
			MyVec3(32.0f, 10.0f, 2.0f),
			MyVec2(0.45f, 0.55f),
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
			1000, 20, 2.0f, MY_TEAM);

		g_livingEntityManager.insertLivingEntityToList(&m_scorpion, -1);
	}

	{
		Material material;

		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		m_skinnedMesh_dude.init(
			m_mesh1Datas[MESH_1_DATA_DUDE],
			m_anim1Datas[ANIM_1_DATA_DUDE],
			m_meshTextures[TEXTURES_MESH_DUDE].Textures,
			m_shaders[SHADER_SKINNED_MESH_1],
			&material);

		//m_skinnedMesh_dude.addInstance(SkinnedMesh1::buildSkinnedMeshInstance(MyVec3(0, 0, 5), MyVec3(0, 180, 0), MyVec3(0.07f), ""));
	}

	{
		Material material;

		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		m_mesh_indiaTowerOfVictory.init(
			m_mesh1Datas[MESH_1_DATA_INDIA_TOWER_OF_VICTORY],
			m_meshTextures[TEXTURES_MESH_INDIA_TOWER_OF_VICTORY].Textures,
			m_shaders[SHADER_MESH],
			&material);

		m_mesh_indiaTowerOfVictory.addInstance(Mesh::buildMeshInstance(MyVec3(0, 0, 10), MyVec3(0), MyVec3(0.3f)));
	}
	
	/*{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Boy03.pak").c_str());

		m_player.init(m_mesh2Datas[MESH_2_DATA_BOY],
			m_anim2Datas[ANIM_2_DATA_BOY],
			resource,
			m_shaders[SHADER_SKINNED_MESH_2], MyVec3(0), MyVec3(0), MyVec3(1), &m_bloodbar_green);
	}*/
	
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
}

void PlayScreen::cloneTrooper()
{
	Trooper* scorpion11 = new Trooper;
	scorpion11->init(TROOPER_SCORPION, MY_TEAM, 100, 10, 2.0f, MyVec3(-20, 0, 2), MyVec3(0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion11, TROOPER_SCORPION);

	Trooper* scorpion12 = new Trooper;
	scorpion12->init(TROOPER_SCORPION, MY_TEAM, 100, 10, 2.0f, MyVec3(-19, 0, 0), MyVec3(0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion12, TROOPER_SCORPION);

	Trooper* scorpion13 = new Trooper;
	scorpion13->init(TROOPER_SCORPION, MY_TEAM, 100, 10, 2.0f, MyVec3(-20, 0, -2), MyVec3(0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion13, TROOPER_SCORPION);

	Trooper* scorpion21 = new Trooper;
	scorpion21->init(TROOPER_SCORPION, ENEMY, 100, 10, 2.0f, MyVec3(20, 0, -2), MyVec3(0, 180, 0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion21, TROOPER_SCORPION);

	Trooper* scorpion22 = new Trooper;
	scorpion22->init(TROOPER_SCORPION, ENEMY, 100, 10, 2.0f, MyVec3(19, 0, 0), MyVec3(0, 180, 0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion22, TROOPER_SCORPION);
	
	Trooper* scorpion23 = new Trooper;
	scorpion23->init(TROOPER_SCORPION, ENEMY, 100, 10, 2.0f, MyVec3(20, 0, 2), MyVec3(0, 180, 0), MyVec3(0.12f), &m_bloodbar_red, &m_bloodbar_green);
	g_livingEntityManager.insertLivingEntityToList(scorpion23, TROOPER_SCORPION);
}

void PlayScreen::resize(int width, int height)
{
	// Core objects
	m_camera_main.resize(width, height);

	// HUD objects
	m_hud.resize(width, height);
}

void PlayScreen::update(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	m_lockedUserInput = false;

	// Core objects
	{
		/*
		MyVec2 delta;
		MyVec3 eye;

		// dpi-dependence
		float CAM_MOVE_FACTOR = 0.03F;
		if (globalUtilObjs->userInput->pointer_Dragging(delta))
		{
			eye = m_camera_main.getEye();
			eye.x -= delta.x * CAM_MOVE_FACTOR;
			eye.z -= delta.y * CAM_MOVE_FACTOR;
			m_camera_main.setEye(eye);
		}
		/**/
		MyVec3 offset = MyVec3(0, 15, 15);
		MyVec3 eye = PositionPlayer + offset;
		m_camera_main.setEye(eye);

		m_camera_main.update();
	}

	// Assets sprite sheets
	for (int i = 0; i < NUM_SPRITE_SHEETS; i++)
	{
		m_spriteSheets[i].update(*globalUtilObjs->timer);
	}

	// Effects
	for (int i = 0; i < NUM_BILLBOARDS; i++)
	{
		m_billboards[i].update(*globalUtilObjs->timer);
	}

	// HUD objects
	m_hud.update(*globalUtilObjs->timer, *globalUtilObjs->userInput);

	// Mesh objects
	m_mesh_indiaTowerOfVictory.update(*globalUtilObjs->timer);
	//m_skinnedMesh_dude.update(*globalUtilObjs->timer);

	int width, height;
	getWindowDimension(width, height);

	m_countTime += globalUtilObjs->timer->getElapsedTime();
	if (m_countTime > 25)
	{
		cloneTrooper();
		m_countTime -= 25;
	}

	if (!m_lockedUserInput)
	{
		//m_scorpion.update(*globalUtilObjs->userInput, *globalUtilObjs->timer, m_camera_main, width, height);
		g_livingEntityManager.update(*globalUtilObjs->userInput, *globalUtilObjs->timer, m_camera_main, width, height);
	}
}

void PlayScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Mesh objects

	m_mesh_terrain.render(m_camera_main);

	{
		Light light;
		light.PosOrDir = MyVec4(0, -1, -1, 0);

		//m_skinnedMesh_dude.render(m_camera_main, &light);
		m_mesh_indiaTowerOfVictory.render(m_camera_main, &light);

		//m_scorpion.render(m_camera_main, light, *globalUtilObjs->spriteBatch);
		g_livingEntityManager.render(m_camera_main, light, *globalUtilObjs->spriteBatch);
	}
		
	// HUD objects
	m_hud.render(*globalUtilObjs->spriteBatch);
}

void PlayScreen::OnPress(const IOnPressListener::Data& data)
{
	if (data.Id == "hud")
	{
		m_lockedUserInput = true;
	}
	else if (data.Id == "btn_hud_fighting")
	{
		m_scorpion.projectile();
	}
}