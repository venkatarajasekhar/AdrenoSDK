
// Always including Utils.h on top
#include "Utils.h"

#include "PlayScreen.h"

//========================================================================================================
//
// Parameters
//
//========================================================================================================

static const MyVec3 MAIN_CAM_POS = MyVec3(10.0f, 15.0f, 25.0f);
static const MyVec3 MAIN_CAM_TARGET = MyVec3(0.0f, 5.0f, 0.0f);
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
		resolveAssetsPath("Shaders/mesh.fs"),
		PosNorTexVertex::ShaderAttribsDesc,
		PosNorTexVertex::NumShaderAttribsDesc);

	// Mesh resources
	m_meshData_scorpion = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/scorpion.model").c_str());

	// Texture resources
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Terrain.pak").c_str());

		m_texture_grass.init(resource.GetTexture("grass"));
	}

	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/Scorpion.pak").c_str());

		m_textures_scorpion = FileMesh1::initTextures(m_meshData_scorpion, resource);
	}

	// Mesh objects
	{
		// Terrain
		std::vector<PosTexVertex> vertices;
		vertices.resize(4);
		vertices[0] = PosTexVertex(MyVec3(-0.5f, +0.0f, -0.5f), MyVec2(0, 0));
		vertices[1] = PosTexVertex(MyVec3(-0.5f, +0.0f, +0.5f), MyVec2(0, 1));
		vertices[2] = PosTexVertex(MyVec3(+0.5f, +0.0f, +0.5f), MyVec2(1, 1));
		vertices[3] = PosTexVertex(MyVec3(+0.5f, +0.0f, -0.5f), MyVec2(1, 0));

		UIntArray indices;
		indices.resize(6);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3;

		m_mesh_terrain.init(vertices, indices, m_shader_terrain, &m_texture_grass, MyVec3(0), MyVec3(0), MyVec3(20));
	}

	{
		Material material;

		material.Ambient = MyVec3(0.05f, 0.05f, 0.05f);
		material.Diffuse = MyVec4(1.0f, 0.5f, 0.5f, 1.0f);
		material.Specular = MyVec4(0.5f, 0.5f, 0.5f, 1.0f);
		material.Shininess = 16.0f;

		m_mesh_scorpion.init(m_meshData_scorpion, m_textures_scorpion, m_shader_mesh, material, MyVec3(0), MyVec3(0), MyVec3(0.5f));
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
	m_camera_main.update();

	// Mesh objects
	m_mesh_terrain.update(*globalUtilObjs->timer);
	m_mesh_scorpion.update();
}

void PlayScreen::render(void* utilObjs)
{
	m_mesh_terrain.render(m_camera_main);

	{
		Light light;
		light.LightPos = MyVec3(50.0f, 50.0f, 50.0f);

		m_mesh_scorpion.render(m_camera_main, light);
	}
}