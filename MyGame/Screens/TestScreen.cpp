
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

//========================================================================================================
//
// PlayScreen class
//
//========================================================================================================

PlayScreen::PlayScreen(ScreenManager* screenManager)
	: Screen(screenManager)
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

#pragma region Init assets

void PlayScreen::initAssets()
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
	m_mesh1Datas[MESH_1_DATA_INDIA_TOWER_OF_VICTORY] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/india_tower_of_victory.model").c_str());
	m_mesh1Datas[MESH_1_DATA_DUDE] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/dude.model").c_str());

	// Assets mesh 2 datas
	m_mesh2Datas[MESH_2_DATA_BOY].Load(resolveAssetsPath("Meshes/Boy03.mesh").c_str());
	m_mesh2Datas[MESH_2_DATA_DMAN].Load(resolveAssetsPath("Meshes/Dman.mesh").c_str());

	// Assets anim 1 datas
	m_anim1Datas[ANIM_1_DATA_SCORPION] = Adreno::FrmLoadAnimationFromFile(resolveAssetsPath("Meshes/scorpion.anim").c_str());
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
		resource.LoadFromFile(resolveAssetsPath("Textures/india_tower_of_victory.pak").c_str());

		m_meshTextures[TEXTURES_MESH_INDIA_TOWER_OF_VICTORY].init(m_mesh1Datas[MESH_1_DATA_INDIA_TOWER_OF_VICTORY], resource);
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/dude.pak").c_str());

		m_meshTextures[TEXTURES_MESH_DUDE].init(m_mesh1Datas[MESH_1_DATA_DUDE], resource);
	}
}

#pragma endregion

void PlayScreen::init()
{
	// Core objects
	m_camera_main.init(MAIN_CAM_POS, MAIN_CAM_TARGET, 45.0f, 0.1f, MAIN_CAM_FAR);

	initAssets();

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
			MyVec2(100),
			properties);
	}

	{
		Material material;

		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		m_testMoving.init(
			m_mesh1Datas[MESH_1_DATA_DUDE],
			m_anim1Datas[ANIM_1_DATA_DUDE],
			m_meshTextures[TEXTURES_MESH_DUDE].Textures,
			m_shaders[SHADER_SKINNED_MESH_1],
			material);
	}
}

void PlayScreen::resize(int width, int height)
{
	// Core objects
	m_camera_main.resize(width, height);

	// HUD objects
}

void PlayScreen::update(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

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

		/*
		MyVec3 offset = MyVec3(0, 15, 15);
		MyVec3 eye = PositionPlayer + offset;
		m_camera_main.setEye(eye);
		/**/

		m_camera_main.update();
	}

	// Assets sprite sheets
	for (int i = 0; i < NUM_SPRITE_SHEETS; i++)
	{
		m_spriteSheets[i].update(*globalUtilObjs->timer);
	}

	// Effects

	// HUD objects

	// Mesh objects
	m_testMoving.update(*globalUtilObjs->userInput, *globalUtilObjs->timer, m_camera_main);
}

void PlayScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Mesh objects
	m_mesh_terrain.render(m_camera_main);

	{
		Light light;
		light.PosOrDir = MyVec4(0, -1, -1, 0);

		m_testMoving.render(m_camera_main, light);
	}
	

	// Effects objects
		
	// HUD objects
}

void PlayScreen::OnPress(const IOnPressListener::Data& data)
{
}