
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
	// Assets mesh textures
	FileMesh1::destroyTextures(m_mesh1Datas[MESH_1_DATA_SCORPION], m_meshTextures[TEXTURES_MESH_SCORPION]);

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

	// Assets mesh 1 datas
	m_mesh1Datas[MESH_1_DATA_SCORPION] = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/scorpion.model").c_str());

	// Assets mesh 2 datas
	m_mesh2Datas[MESH_2_DATA_BOY].Load(resolveAssetsPath("Meshes/Boy03.mesh").c_str());

	// Assets anim 1 datas
	m_anim1Datas[ANIM_1_DATA_SCORPION] = Adreno::FrmLoadAnimationFromFile(resolveAssetsPath("Meshes/scorpion.anim").c_str());

	// Assets anim 2 datas
	FrmReadAnimation(resolveAssetsPath("Meshes/Boy03.anim").c_str(), &m_anim2Datas[ANIM_2_DATA_BOY]);

	// Assets mesh textures
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Scorpion.pak").c_str());

		m_meshTextures[TEXTURES_MESH_SCORPION] = FileMesh1::initTextures(m_mesh1Datas[MESH_1_DATA_SCORPION], resource);
	}

	// Mesh objects
	{
		FlatTerrainProperties properties = 
		{
			MyVec3(8.0f, 2.0f, 1.0f),
			MyVec2(0.45f, 0.55f),
		};

		m_mesh_terrain.init(
			m_shaders[SHADER_TERRAIN], 
			m_textures[TEXTURE_TERRAIN_DIFF_1], 
			m_textures[TEXTURE_TERRAIN_DIFF_2], 
			m_textures[TEXTURE_TERRAIN_BLEND], 
			MyVec2(20), 
			properties);
	}
	
	{
		Material material;

		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 0.5f, 0.5f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		m_mesh_scorpion.init(
			m_mesh1Datas[MESH_1_DATA_SCORPION], 
			m_meshTextures[TEXTURES_MESH_SCORPION], 
			m_shaders[SHADER_MESH],
			&material);

		m_mesh_scorpion.addInstance(Mesh::buildMeshInstance(MyVec3(0), MyVec3(0), MyVec3(0.1f)));
		m_mesh_scorpion.addInstance(Mesh::buildMeshInstance(MyVec3(5, 0, 6), MyVec3(0, 45, 0), MyVec3(0.1f)));
		m_mesh_scorpion.addInstance(Mesh::buildMeshInstance(MyVec3(-4, 0, 3), MyVec3(0, 120, 0), MyVec3(0.1f)));

		/*
		m_skinnedMesh_scorpion.init(
			m_mesh1Datas[MESH_1_DATA_SCORPION],
			m_anim1Datas[ANIM_1_DATA_SCORPION],
			m_meshTextures[TEXTURES_MESH_SCORPION],
			m_shaders[SHADER_SKINNED_MESH_1],
			MyVec3(0),
			MyVec3(0),
			MyVec3(1.0f),
			&material);
		/**/
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Boy03.pak").c_str());

		Material material;

		material.Ambient = MyVec3(0.1f, 0.1f, 0.1f);
		material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Specular = MyVec4(0.4f, 0.4f, 0.4f, 1.0f);
		material.Shininess = 16.0f;

		/*
		m_skinnedMesh_boy.init(
			m_mesh2Datas[MESH_2_DATA_BOY], 
			m_anim2Datas[ANIM_2_DATA_BOY], 
			resource, 
			m_shaders[SHADER_SKINNED_MESH_2],
			MyVec3(0),
			MyVec3(0),
			MyVec3(1.0f), 
			&material);
		/**/
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
		MyVec3 eye;

		// dpi-dependence
		float CAM_MOVE_FACTOR = 0.03F;
		if (globalUtilObjs->userInput->pointer_Dragging(delta))
		{
			eye = m_camera_main.getEye();
			eye.x += delta.x * CAM_MOVE_FACTOR;
			eye.z += delta.y * CAM_MOVE_FACTOR;
			m_camera_main.setEye(eye);
		}

		m_camera_main.update();
	}
	
	// Mesh objects
	m_mesh_scorpion.update(*globalUtilObjs->timer);
	//m_skinnedMesh_scorpion.update(*globalUtilObjs->timer);
	//m_skinnedMesh_boy.update(*globalUtilObjs->timer);
}

void PlayScreen::render(void* utilObjs)
{
	m_mesh_terrain.render(m_camera_main);

	{
		Light light;
		light.PosOrDir = MyVec4(0, -1, -1, 0);

		m_mesh_scorpion.render(m_camera_main, &light);
		//m_skinnedMesh_scorpion.render(m_camera_main, &light);
		//m_skinnedMesh_boy.render(m_camera_main, &light);
	}
}