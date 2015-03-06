
// Always including Utils.h on top
#include "Utils.h"

#include "PlayScreen.h"

//========================================================================================================
//
// Parameters
//
//========================================================================================================

// View model scorpion
//static const MyVec3 MAIN_CAM_POS = MyVec3(10.0f, 15.0f, 25.0f);
//static const MyVec3 MAIN_CAM_TARGET = MyVec3(0.0f, 5.0f, 0.0f);

// View model boy
//static const MyVec3 MAIN_CAM_POS = MyVec3(2.0f, 1.3f, 3.0f);
//static const MyVec3 MAIN_CAM_TARGET = MyVec3(0.0f, 1.0f, 0.0f);

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
	// Texture resources
	FileMesh1::destroyTextures(m_meshData_scorpion, m_textures_scorpion);

	// Mesh resources
	Adreno::FrmDestroyLoadedModel(m_meshData_scorpion);
	Adreno::FrmDestroyLoadedAnimation(m_animData_scorpion);

	delete m_animData_boy;
}

void PlayScreen::init()
{
	// Core objects
	m_camera_main.init(MAIN_CAM_POS, MAIN_CAM_TARGET, 45.0f, 0.1f, MAIN_CAM_FAR);

	// Shader resources
	m_shader_terrain.init(
		resolveAssetsPath("Shaders/terrain.vs"),
		resolveAssetsPath("Shaders/terrain.fs"),
		PosTexVertex::ShaderAttribsDesc,
		PosTexVertex::NumShaderAttribsDesc);

	m_shader_mesh.init(
		resolveAssetsPath("Shaders/mesh.vs"),
		resolveAssetsPath("Shaders/PhongShading.fs"),
		PosNorTexVertex::ShaderAttribsDesc,
		PosNorTexVertex::NumShaderAttribsDesc);

	m_shader_skinnedMesh1.init(
		resolveAssetsPath("Shaders/skinnedMesh1.vs"),
		resolveAssetsPath("Shaders/PhongShading.fs"),
		SkinnedVertex::ShaderAttribsDesc,
		SkinnedVertex::NumShaderAttribsDesc);

	m_shader_skinnedMesh2.init(
		resolveAssetsPath("Shaders/skinnedMesh2.vs"),
		resolveAssetsPath("Shaders/PhongShading.fs"),
		SkinnedVertex::ShaderAttribsDesc,
		SkinnedVertex::NumShaderAttribsDesc);

	// Mesh resources
	m_meshData_scorpion = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/scorpion.model").c_str());
	m_animData_scorpion = Adreno::FrmLoadAnimationFromFile(resolveAssetsPath("Meshes/scorpion.anim").c_str());

	m_meshData_boy.Load(resolveAssetsPath("Meshes/Boy03.mesh").c_str());
	FrmReadAnimation(resolveAssetsPath("Meshes/Boy03.anim").c_str(), &m_animData_boy);

	//m_meshData_boy.Load(resolveAssetsPath("Meshes/Dman.mesh").c_str());
	//FrmReadAnimation(resolveAssetsPath("Meshes/Dman.anim").c_str(), &m_animData_boy);

	// Texture resources
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Terrain.pak").c_str());

		m_textures_terrain[0].init(resource.GetTexture("diffuse_1"));
		m_textures_terrain[1].init(resource.GetTexture("diffuse_2"));
		m_textures_terrain[2].init(resource.GetTexture("blend"));
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Scorpion.pak").c_str());

		m_textures_scorpion = FileMesh1::initTextures(m_meshData_scorpion, resource);
	}

	// Mesh objects
	{
		FlatTerrainProperties properties = 
		{
			MyVec3(8.0f, 2.0f, 1.0f),
			MyVec2(0.45f, 0.55f),
		};

		m_mesh_terrain.init(m_shader_terrain, m_textures_terrain[0], m_textures_terrain[1], m_textures_terrain[2], MyVec2(20), properties);
	}
	
	{
		Material material;

		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 0.5f, 0.5f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		//m_mesh_scorpion.init(m_meshData_scorpion, m_textures_scorpion, m_shader_mesh, MyVec3(0), MyVec3(0), MyVec3(0.5f), &material);
		/*m_skinnedMesh_scorpion.init(
			m_meshData_scorpion,
			m_animData_scorpion,
			m_textures_scorpion,
			m_shader_skinnedMesh1,
			MyVec3(0),
			MyVec3(0),
			MyVec3(1.0f),
			&material);*/
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Boy03.pak").c_str());
		//resource.LoadFromFile(resolveAssetsPath("Textures/Dman.pak").c_str());

		Material material;

		material.Ambient = MyVec3(0.1f, 0.1f, 0.1f);
		material.Diffuse = MyVec4(1.0f, 1.0f, 1.0f, 1.0f);
		material.Specular = MyVec4(0.4f, 0.4f, 0.4f, 1.0f);
		material.Shininess = 16.0f;

		m_skinnedMesh_boy.init(m_meshData_boy, m_animData_boy, resource, m_shader_skinnedMesh2,
			MyVec3(0),
			MyVec3(0),
			MyVec3(1.0f), &material);
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
	//m_mesh_scorpion.update(*globalUtilObjs->timer);
	//m_skinnedMesh_scorpion.update(*globalUtilObjs->timer);
	m_skinnedMesh_boy.update(*globalUtilObjs->timer);
}

void PlayScreen::render(void* utilObjs)
{
	m_mesh_terrain.render(m_camera_main);

	{
		Light light;
		light.PosOrDir = MyVec4(0, -1, -1, 0);

		//m_mesh_scorpion.render(m_camera_main, &light);
		//m_skinnedMesh_scorpion.render(m_camera_main, &light);
		m_skinnedMesh_boy.render(m_camera_main, &light);
	}
}