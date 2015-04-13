
#include "Layer_World.h"

//========================================================================================================
//
// Parameters
//
//========================================================================================================

static const float  MAIN_CAM_FAR       = 100.0f;
static const MyVec3 INITIAL_PLAYER_POS = MyVec3(0);

//========================================================================================================
//
// Layer_World
//
//========================================================================================================

Layer_World::Layer_World()
{
}

Layer_World::~Layer_World()
{
}

// Core functions

void Layer_World::init(Layer_World::InitBundle& bundle)
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

	// Core objects
	m_camera_main.init(INITIAL_PLAYER_POS, 45.0f, 0.1f, MAIN_CAM_FAR);

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
			bundle.MapCenter,
			bundle.MapSize,
			properties);
	}

	// Graphics objects
	m_bloodBar[BLOOD_BAR_MY_TEAM].init(m_textures[TEXTURE_BLOODBAR_GREEN_FORE], m_textures[TEXTURE_BLOODBAR_GREEN_BACK]);
	m_bloodBar[BLOOD_BAR_ENEMY].init(m_textures[TEXTURE_BLOODBAR_RED_FORE], m_textures[TEXTURE_BLOODBAR_RED_BACK]);

	// Game objects
	m_towerPool.init(m_shaders[SHADER_MESH], m_bloodBar[BLOOD_BAR_MY_TEAM], m_bloodBar[BLOOD_BAR_ENEMY], m_livingEnts);
	m_pawnPool.init(m_shaders[SHADER_SKINNED_MESH_1], m_bloodBar[BLOOD_BAR_MY_TEAM], m_bloodBar[BLOOD_BAR_ENEMY], m_livingEnts);
	m_heroPool.init(m_shaders[SHADER_SKINNED_MESH_1], m_bloodBar[BLOOD_BAR_MY_TEAM], m_bloodBar[BLOOD_BAR_ENEMY], m_livingEnts, m_mesh_terrain);
}

void Layer_World::resize(int width, int height)
{
	// Core objects
	m_camera_main.resize(width, height);
}

void Layer_World::update(Timer& timer, UserInput& userInput)
{
	// Core objects
	m_camera_main.update(timer, userInput);

	// Mesh objects
	m_mesh_terrain.update(timer, userInput, m_camera_main);

	// Game objects
	for (auto i = m_livingEnts.begin(); i != m_livingEnts.end(); ++i)
	{
		(*i)->update(timer);
	}

	m_towerPool.update(timer);
	m_pawnPool.update(timer);
	m_heroPool.update(timer);
}

void Layer_World::render(SpriteBatch& spriteBatch)
{
	// Mesh objects
	m_mesh_terrain.render(m_camera_main);

	// Game objects
	{
		Light light;
		light.PosOrDir = MyVec4(0, -1, -1, 0);

		m_towerPool.render(m_camera_main, light);
		m_pawnPool.render(m_camera_main, light);
		m_heroPool.render(m_camera_main, light);

		for (auto i = m_livingEnts.begin(); i != m_livingEnts.end(); ++i)
		{
			(*i)->render(spriteBatch, m_camera_main, light);
		}
	}
}