
#include "MainGame.h"
#include <FrmPackedResourceGLES.h>

//========================================================================================================
//
// Parameters
//
//========================================================================================================

static const MyVec3 MAIN_CAM_POS = MyVec3(10.0f, 15.0f, 25.0f);
static const MyVec3 MAIN_CAM_TARGET = MyVec3(0.0f, 5.0f, 0.0f);
static const float MAIN_CAM_FAR = 100.0f;

#pragma region Global implementations

//========================================================================================================
//
// Global implementations
//
//========================================================================================================

CFrmApplication* FrmCreateApplicationInstance()
{
	MainGame* app = new MainGame("My Game");
	return app;
}

MyString resolveAssetsPath(const MyString& relativePath)
{
#ifdef WIN32
	return "MyGame-Android/app/src/main/assets/" + relativePath;
#elif defined __ANDROID__
	return relativePath;
#endif
}

#pragma endregion

//========================================================================================================
//
// MainGame implementation
//
//========================================================================================================

#pragma region Constructor

MainGame::MainGame(const CHAR* strName)
	: CFrmApplication(strName),
	m_initialized(false),
	m_updated(false)
{
	g_strWindowTitle = strName;
	g_nWindowWidth = 800;
	g_nWindowHeight = 600;
	g_fAspectRatio = (FLOAT)m_nWidth / (FLOAT)m_nHeight;
}

#pragma endregion

BOOL MainGame::Initialize()
{
	// Init here

	// Mesh resources

	// Shader resources
	m_shader_sprite.init(
		resolveAssetsPath("Shaders/sprite.vs"),
		resolveAssetsPath("Shaders/sprite.fs"),
		Pos2TexVertex::ShaderAttribsDesc,
		Pos2TexVertex::NumShaderAttribsDesc);

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

	// Core objects
	m_spriteBatch.init(m_shader_sprite);
	m_camera_main.init(MAIN_CAM_POS, MAIN_CAM_TARGET, 45.0f, 0.1f, MAIN_CAM_FAR);

	// Mesh resources
	m_meshData_scorpion = Adreno::FrmLoadModelFromFile(resolveAssetsPath("Meshes/scorpion.model").c_str());

	// Texture resources
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/textures.pak").c_str());

		m_texture_grass.init(resource.GetTexture("Brick"));
		m_texture_snowman.init(resource.GetTexture("snowman"));
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
	

	m_initialized = true;

	return Resize();
}

BOOL MainGame::Resize()
{
	if (!m_initialized) return TRUE;

	// Resize here

	// Core objects
	m_spriteBatch.resize(m_nWidth, m_nHeight);
	m_camera_main.resize(m_nWidth, m_nHeight);

	glViewport( 0, 0, m_nWidth, m_nHeight );

	return TRUE;
}

VOID MainGame::Destroy()
{
	if (!m_initialized) return;

	// Destroy here

	// Texture resources
	FileMesh1::destroyTextures(m_meshData_scorpion, m_textures_scorpion);

	// Mesh resources
	Adreno::FrmDestroyLoadedModel(m_meshData_scorpion);
}

VOID MainGame::Update()
{
	if (!m_initialized) return;

	// Update here

	// Core objects
	m_timer.update();
	m_spriteBatch.update();
	m_camera_main.update();

	// Mesh objects
	m_mesh_terrain.update(m_timer);
	m_mesh_scorpion.update();

	m_updated = true;
}

VOID MainGame::Render()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Clear the backbuffer and depth-buffer
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!m_updated) return;

	// Render here
	
	// Mesh objects
	m_mesh_terrain.render(m_camera_main);

	{
		Light light;

		light.LightPos = MyVec3(50.0f, 50.0f, 50.0f);

		m_mesh_scorpion.render(m_camera_main, light);
	}
	

	// 2D
	m_spriteBatch.renderTexture2D(m_texture_snowman, MyVec2(100, 100), 0, MyVec2(0.5f, 0.5f));
}