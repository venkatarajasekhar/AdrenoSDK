
#include "Layer_World.h"

//========================================================================================================
//
// Parameters
//
//========================================================================================================

static const MyVec3 MAIN_CAM_POS    = MyVec3(0.0f, 10.0f, 10.0f);
static const MyVec3 MAIN_CAM_TARGET = MyVec3(0.0f, 0.0f, 0.0f);
static const float  MAIN_CAM_FAR    = 100.0f;

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

void Layer_World::init()
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
	m_camera_main.init(MAIN_CAM_POS, MAIN_CAM_TARGET, 45.0f, 0.1f, MAIN_CAM_FAR);
}

void Layer_World::resize(int width, int height)
{
	// Core objects
	m_camera_main.resize(width, height);
}

void Layer_World::update(Timer& timer, UserInput& userInput)
{
	// Core objects
	m_camera_main.update();
}

void Layer_World::render(SpriteBatch& spriteBatch)
{

}