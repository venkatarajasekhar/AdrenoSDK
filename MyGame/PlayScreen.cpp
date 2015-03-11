
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
		resource.LoadFromFile(resolveAssetsPath("Textures/HUD.pak").c_str());

		m_textures[TEXTURE_GREEN_FORE_BLOODBAR].init(resource.GetTexture("green_fore_bloodbar"));
		m_textures[TEXTURE_GREEN_BACK_BLOODBAR].init(resource.GetTexture("green_back_bloodbar"));
		m_textures[TEXTURE_RED_FORE_BLOODBAR].init(resource.GetTexture("red_fore_bloodbar"));
		m_textures[TEXTURE_RED_BACK_BLOODBAR].init(resource.GetTexture("red_back_bloodbar"));
	}

	// Core objects
	m_bloodbar_green.init(m_textures[TEXTURE_GREEN_FORE_BLOODBAR], m_textures[TEXTURE_GREEN_BACK_BLOODBAR]);
	m_bloodbar_red.init(m_textures[TEXTURE_RED_FORE_BLOODBAR], m_textures[TEXTURE_RED_BACK_BLOODBAR]);

	// Assets mesh 1 datas
	m_mesh1Datas[MESH_1_DATA_SCORPION] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/scorpion.model").c_str());
	m_mesh1Datas[MESH_1_DATA_INDIA_TOWER_OF_VICTORY] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/india_tower_of_victory.model").c_str());

	// Assets mesh 2 datas
	m_mesh2Datas[MESH_2_DATA_BOY].Load(resolveAssetsPath("Meshes/Boy03.mesh").c_str());
	m_mesh2Datas[MESH_2_DATA_DMAN].Load(resolveAssetsPath("Meshes/Dman.mesh").c_str());

	// Assets anim 1 datas
	m_anim1Datas[ANIM_1_DATA_SCORPION] = Adreno::FrmLoadAnimationFromFile(resolveAssetsPath("Meshes/scorpion.anim").c_str());

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

	// Mesh objects
	{
		FlatTerrainProperties properties = 
		{
			MyVec3(32.0f, 10.0f, 2.0f),
			MyVec2(0.45f, 0.55f),
			//MyVec2(1.0f, 1.0f),
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
		material.Diffuse = MyVec4(1.0f, 0.5f, 0.5f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		/*
		m_mesh_scorpion.init(
			m_mesh1Datas[MESH_1_DATA_SCORPION], 
			m_meshTextures[TEXTURES_MESH_SCORPION], 
			m_shaders[SHADER_MESH],
			&material);

		m_mesh_scorpion.addInstance(Mesh::buildMeshInstance(MyVec3(0), MyVec3(0), MyVec3(0.1f)));
		m_mesh_scorpion.addInstance(Mesh::buildMeshInstance(MyVec3(5, 0, 6), MyVec3(0, 45, 0), MyVec3(0.1f)));
		m_mesh_scorpion.addInstance(Mesh::buildMeshInstance(MyVec3(-4, 0, 3), MyVec3(0, 120, 0), MyVec3(0.1f)));
		/**/

		/*
		m_skinnedMesh_scorpion.init(
			m_mesh1Datas[MESH_1_DATA_SCORPION],
			m_anim1Datas[ANIM_1_DATA_SCORPION],
			m_meshTextures[TEXTURES_MESH_SCORPION].Textures,
			m_shaders[SHADER_SKINNED_MESH_1],
			&material);

		m_skinnedMesh_scorpion.addInstance(SkinnedMesh1::buildSkinnedMeshInstance(MyVec3(0, 0, 5), MyVec3(0), MyVec3(0.2f), ""));
		m_skinnedMesh_scorpion.addInstance(SkinnedMesh1::buildSkinnedMeshInstance(MyVec3(5, 0, 6), MyVec3(0, 45, 0), MyVec3(0.2f), ""));
		m_skinnedMesh_scorpion.addInstance(SkinnedMesh1::buildSkinnedMeshInstance(MyVec3(-4, 0, 3), MyVec3(0, 120, 0), MyVec3(0.2f), ""));
		/**/
	}

	{
		Material material;

		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		/*
		m_mesh_indiaTowerOfVictory.init(
			m_mesh1Datas[MESH_1_DATA_INDIA_TOWER_OF_VICTORY],
			m_meshTextures[TEXTURES_MESH_INDIA_TOWER_OF_VICTORY].Textures,
			m_shaders[SHADER_MESH],
			&material);

		m_mesh_indiaTowerOfVictory.addInstance(Mesh::buildMeshInstance(MyVec3(0), MyVec3(0), MyVec3(0.4f)));
		*/
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Boy03.pak").c_str());

		/*
		Material material;

		material.Ambient = MyVec3(0.1f, 0.1f, 0.1f);
		material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Specular = MyVec4(0.4f, 0.4f, 0.4f, 1.0f);
		material.Shininess = 16.0f;

		m_skinnedMesh_boy.init(
			m_mesh2Datas[MESH_2_DATA_BOY], 
			m_anim2Datas[ANIM_2_DATA_BOY], 
			resource, 
			m_shaders[SHADER_SKINNED_MESH_2],
			&material);

		m_skinnedMesh_boy.addInstance(SkinnedMesh2::buildSkinnedMeshInstance(MyVec3(0), MyVec3(0), MyVec3(1.0f), ""));
		m_skinnedMesh_boy.addInstance(SkinnedMesh2::buildSkinnedMeshInstance(MyVec3(5, 0, 6), MyVec3(0, 45, 0), MyVec3(1.0f), ""));
		m_skinnedMesh_boy.addInstance(SkinnedMesh2::buildSkinnedMeshInstance(MyVec3(-4, 0, 3), MyVec3(0, 120, 0), MyVec3(1.0f), ""));
		/**/
		m_player.init(m_mesh2Datas[MESH_2_DATA_BOY],
			m_anim2Datas[ANIM_2_DATA_BOY],
			resource,
			m_shaders[SHADER_SKINNED_MESH_2], MyVec3(0), MyVec3(0), MyVec3(1));
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Dman.pak").c_str());
		
		m_dmanManager.init(m_mesh2Datas[MESH_2_DATA_DMAN],
			m_anim2Datas[ANIM_2_DATA_DMAN],
			resource,
			m_shaders[SHADER_SKINNED_MESH_2]);

		/*Dman* dman = new Dman;
		dman->init(0, MyVec3(-100, 0, 0), MyVec3(0), MyVec3(0.7));
		m_dmanManager.insertDmanToList(dman);*/
	}
}

void PlayScreen::cloneDman()
{
	if (m_dmanManager.getNDman() < 10)
	{
		Dman* dman1 = new Dman;
		dman1->init(0, MyVec3(-50, 0, 0), MyVec3(0), MyVec3(0.7));
		m_dmanManager.insertDmanToList(dman1);

		Dman* dman2 = new Dman;
		dman2->init(1, MyVec3(50, 0, 0), MyVec3(0), MyVec3(0.7));
		m_dmanManager.insertDmanToList(dman2);
	}
}

void PlayScreen::resize(int width, int height)
{
	m_camera_main.resize(width, height);
}

void PlayScreen::update(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	// Core objects

	{
		// Camera
		MyVec2 delta;
		//MyVec3 eye;

		// dpi-dependence
		/*
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
	
	// Mesh objects
	//m_mesh_scorpion.update(*globalUtilObjs->timer);
	//m_skinnedMesh_scorpion.update(*globalUtilObjs->timer);
	//m_skinnedMesh_boy.update(*globalUtilObjs->timer);
	//m_mesh_indiaTowerOfVictory.update(*globalUtilObjs->timer);

	int width, height;
	getWindowDimension(width, height);

	m_countTime += globalUtilObjs->timer->getElapsedTime();
	if (m_countTime > 4)
	{
		cloneDman();
		m_countTime -= 4;
	}

	m_player.update(*globalUtilObjs->userInput, *globalUtilObjs->timer, m_camera_main, width, height);
	m_dmanManager.update(*globalUtilObjs->timer);
}

void PlayScreen::render(void* utilObjs)
{
	GLOBAL_UTIL_OBJS* globalUtilObjs = (GLOBAL_UTIL_OBJS*)utilObjs;

	m_mesh_terrain.render(m_camera_main);

	{
		Light light;
		light.PosOrDir = MyVec4(0, -1, -1, 0);

		//m_mesh_scorpion.render(m_camera_main, &light);
		//m_skinnedMesh_scorpion.render(m_camera_main, &light);
		//m_skinnedMesh_boy.render(m_camera_main, &light);

		//m_mesh_indiaTowerOfVictory.render(m_camera_main, &light);

		m_player.render(m_camera_main, light);
		m_dmanManager.render(m_camera_main, light);
	}

	{
		static float health = 1.0f;
		health *= 0.999f;
		m_bloodbar_green.render(*globalUtilObjs->spriteBatch, m_camera_main, PositionPlayer, health);
		//m_bloodbar_red.render(*globalUtilObjs->spriteBatch, m_camera_main, PositionPlayer, health);
	}
}