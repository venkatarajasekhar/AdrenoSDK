
#include "Layer_World.h"

#pragma region Constants

//========================================================================================================
//
// Constants
//
//========================================================================================================

static const float  MAIN_CAM_FAR       = 100.0f;
static const MyVec3 INITIAL_PLAYER_POS = MyVec3(0);

#pragma endregion

#pragma region Helper functions

//========================================================================================================
//
// Helper functions
//
//========================================================================================================

static bool isPressTerrain(UserInput& userInput, Camera& camera, MyVec3& pressedPoint)
{
	MyVec2 screenPos;
	bool isPressed = userInput.pointer_Releasing(screenPos);

	if (isPressed)
	{
		Plane terrainPlane = { 0.0f, MyVec3(0, 1, 0) };
		int width, height;
		getWindowDimension(width, height);

		// Convert pressed point on screen to point in world
		Ray ray = createRayInWorld(screenPos, width, height, camera.getView(), camera.getProj());
		pressedPoint = intersect(ray, terrainPlane);
	}

	return isPressed;
}

#pragma endregion

//========================================================================================================
//
// Layer_World class
//
//========================================================================================================

Layer_World::Layer_World()
	: m_selectedGameObj(nullptr)
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
		resolveAssetsPath("Shaders/textured_mesh.vs"),
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

	m_shaders[SHADER_TEXTURED_MESH].init(
		resolveAssetsPath("Shaders/textured_mesh.vs"),
		resolveAssetsPath("Shaders/textured_mesh.fs"),
		PosTexVertex::ShaderAttribsDesc,
		PosTexVertex::NumShaderAttribsDesc);

	m_shaders[SHADER_TEST_SHAPE].init(
		resolveAssetsPath("Shaders/test_shape.vs"),
		resolveAssetsPath("Shaders/test_shape.fs"),
		PosNorTexVertex::ShaderAttribsDesc,
		PosNorTexVertex::NumShaderAttribsDesc);

	// Assets textures
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Textures/terrain.pak").c_str());

		m_textures[TEXTURE_TERRAIN_DIFF_1].init(resource.GetTexture("diffuse_1"));
		m_textures[TEXTURE_TERRAIN_DIFF_2].init(resource.GetTexture("diffuse_2"));
		m_textures[TEXTURE_TERRAIN_BLEND].init(resource.GetTexture("blend"));
		m_textures[TEXTURE_TERRAIN_DECAL_SELECTED].init(resource.GetTexture("decal_selected"));
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

		m_spriteSheets[SPRITE_SHEET_ENERGY_BALL].init(resource.GetTexture("energy_ball"), 8, MyIVec2(3, 1), MyIVec2(100, 100));
		m_spriteSheets[SPRITE_SHEET_BULLET].init(resource.GetTexture("energy_ball"), 8, MyIVec2(3, 1), MyIVec2(100, 100));
	}

	// Assets mesh data
	m_mesh1Datas[MESH_1_DATA_SHOP].init(resolveAssetsPath("Meshes/Accessories/shop/shop.model"));

	// Assets mesh texture
	{
		CFrmPackedResourceGLES resource;
		resource.LoadFromFile(resolveAssetsPath("Meshes/Accessories/shop/shop.pak").c_str());
		m_meshTextures[TEXTURES_MESH_SHOP].init(m_mesh1Datas[MESH_1_DATA_SHOP], resource);
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

	m_selectedDecal.init(
		&m_textures[TEXTURE_TERRAIN_DECAL_SELECTED],
		m_shaders[SHADER_TEXTURED_MESH],
		MyVec3(0, 0.5f, 0),
		MyVec3(),
		MyVec2(3.0f));

	m_shop.init(
		m_mesh1Datas[MESH_1_DATA_SHOP],
		m_meshTextures[TEXTURES_MESH_SHOP],
		m_shaders[SHADER_MESH],
		MyVec3(-39.195f, 0, -14.0f),
		MyVec3(0, -45, 0),
		MyVec3(1.5f),
		m_selectedDecal);
	m_shop.addPressListener(bundle.ShopListener);

	// Graphics objects
	m_bloodBar[BLOOD_BAR_MY_TEAM].init(m_textures[TEXTURE_BLOODBAR_GREEN_FORE], m_textures[TEXTURE_BLOODBAR_GREEN_BACK]);
	m_bloodBar[BLOOD_BAR_ENEMY].init(m_textures[TEXTURE_BLOODBAR_RED_FORE], m_textures[TEXTURE_BLOODBAR_RED_BACK]);

	m_billboards[BILLBOARD_ENERGY_BALL].init(
		&m_spriteSheets[SPRITE_SHEET_ENERGY_BALL],
		m_shaders[SHADER_BILLBOARD],
		MyVec3(0),
		MyVec2(1.5f),
		0);

	m_billboards[BILLBOARD_BULLET].init(
		&m_spriteSheets[SPRITE_SHEET_ENERGY_BALL],
		m_shaders[SHADER_BILLBOARD],
		MyVec3(0),
		MyVec2(0.5f),
		0);

	// Audio assets
	m_audios[AUDIO_MYPAWN_ATTACK].init(resolveAssetsPath("Audios/MyPawnAttack.wav"));
	m_audios[AUDIO_MYPAWN_ATTACK].setVolume(0.1f);
	m_audios[AUDIO_MYPAWN_DEATH].init(resolveAssetsPath("Audios/MyPawnDeath.wav"));
	m_audios[AUDIO_MYPAWN_DEATH].setVolume(0.3f);
	m_audios[AUDIO_ENEMYPAWN_ATTACK].init(resolveAssetsPath("Audios/EnemyPawnAttack.wav"));
	m_audios[AUDIO_ENEMYPAWN_ATTACK].setVolume(0.1f);
	m_audios[AUDIO_ENEMYPAWN_DEATH].init(resolveAssetsPath("Audios/EnemyPawnDeath.wav"));
	m_audios[AUDIO_ENEMYPAWN_DEATH].setVolume(0.3f);

	m_audios[AUDIO_MYIFV_ATTACK].init(resolveAssetsPath("Audios/MyTankAttack.wav"));
	m_audios[AUDIO_MYIFV_ATTACK].setVolume(0.2f);
	m_audios[AUDIO_ENEMYIFV_ATTACK].init(resolveAssetsPath("Audios/EnemyTankAttack.wav"));
	m_audios[AUDIO_ENEMYIFV_ATTACK].setVolume(0.2f);
	m_audios[AUDIO_IFV_DEATH].init(resolveAssetsPath("Audios/TankDeath.wav"));
	m_audios[AUDIO_IFV_DEATH].setVolume(0.3f);

	m_audios[AUDIO_MYTOWER_ATTACK].init(resolveAssetsPath("Audios/MyTowerAttack.wav"));
	m_audios[AUDIO_MYTOWER_ATTACK].setVolume(0.5f);
	m_audios[AUDIO_ENEMYTOWER_ATTACK].init(resolveAssetsPath("Audios/EnemyTowerAttack.wav"));
	m_audios[AUDIO_ENEMYTOWER_ATTACK].setVolume(0.5f);
	m_audios[AUDIO_TOWER_DEATH].init(resolveAssetsPath("Audios/TowerDeath.wav"));
	m_audios[AUDIO_TOWER_DEATH].setVolume(0.5f);

	m_audios[AUDIO_MYHERO_ATTACK].init(resolveAssetsPath("Audios/MyHeroAttack.wav"));
	m_audios[AUDIO_MYHERO_DEATH].init(resolveAssetsPath("Audios/MyHeroDeath.wav"));
	m_audios[AUDIO_ENEMYHERO_ATTACK].init(resolveAssetsPath("Audios/EnemyHeroAttack.wav"));
	m_audios[AUDIO_ENEMYHERO_ATTACK2].init(resolveAssetsPath("Audios/EnemyHeroAttack2.wav"));
	m_audios[AUDIO_ENEMYHERO_DEATH].init(resolveAssetsPath("Audios/EnemyHeroDeath.wav"));

	m_audios[AUDIO_HEALTH_SKILL].init(resolveAssetsPath("Audios/HealthSkill.wav"));
	m_audios[AUDIO_UPGRADE_SKILL].init(resolveAssetsPath("Audios/UpgradeSkill.wav"));

	// Game objects
	m_towerPool.init(
		m_shaders[SHADER_MESH],
		m_bloodBar[BLOOD_BAR_MY_TEAM],
		m_bloodBar[BLOOD_BAR_ENEMY],
		m_selectedDecal,
		m_billboards[BILLBOARD_ENERGY_BALL],
		m_projectilePool,
		m_livingEnts,
		m_audios,
		bundle.GameOverListener);

	m_pawnPool.init(
		m_shaders[SHADER_SKINNED_MESH_1], 
		m_bloodBar[BLOOD_BAR_MY_TEAM], 
		m_bloodBar[BLOOD_BAR_ENEMY], 
		m_selectedDecal,
		m_livingEnts,
		m_audios);

	m_iFVPool.init(
		m_shaders[SHADER_SKINNED_MESH_1],
		m_bloodBar[BLOOD_BAR_MY_TEAM],
		m_bloodBar[BLOOD_BAR_ENEMY],
		m_selectedDecal,
		m_billboards[BILLBOARD_BULLET],
		m_projectilePool,
		m_livingEnts,
		m_audios);

	m_heroPool.init(
		m_shaders[SHADER_SKINNED_MESH_1], 
		m_shaders[SHADER_BILLBOARD],
		m_shaders[SHADER_TEST_SHAPE],
		m_bloodBar[BLOOD_BAR_MY_TEAM], 
		m_bloodBar[BLOOD_BAR_ENEMY], 
		m_selectedDecal,
		m_livingEnts, 
		m_audios,
		m_mesh_terrain);

	m_projectilePool.init();
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

	// Assets
	for (int i = 0; i < NUM_SPRITE_SHEETS; i++)
	{
		m_spriteSheets[i].update(timer);
	}

	// Graphics objects
	for (int i = 0; i < NUM_BILLBOARDS; i++)
	{
		m_billboards[i].update(timer);
	}

	// Mesh objects
	{
		MyVec3 pressedPoint;
		bool isPressed = isPressTerrain(userInput, m_camera_main, pressedPoint);

		//---------------- Test ------------------------------------------------------------------------------------------
		//smartLog("Pressed at: " + toString(pressedPoint.x) + " " + toString(pressedPoint.y) + " " + toString(pressedPoint.z));
		//---------------- Test ------------------------------------------------------------------------------------------

		m_shop.update(timer, isPressed, pressedPoint);

		for (auto i = m_livingEnts.begin(); i != m_livingEnts.end(); ++i)
		{
			LivingEntity* lEnt = (*i);
			if (isPressed) lEnt->deselect(); 
			if (lEnt->inUse() && (lEnt->getTeamType() == TEAM_TYPE_ENEMY))
			{
				if (lEnt->isSelect(isPressed, pressedPoint))
				{
					if (m_selectedGameObj != nullptr)
					{
						m_selectedGameObj->deselect();
					}

					lEnt->select();
					m_selectedGameObj = lEnt;
					isPressed = false;
					break;
				}
			}
		}

		m_mesh_terrain.update(timer, isPressed, pressedPoint);
	}

	m_selectedDecal.update(timer);
	
	// Game objects
	m_towerPool.update(timer);
	m_pawnPool.update(timer);
	m_iFVPool.update(timer);
	m_heroPool.update(timer);
	m_projectilePool.update(timer);

	for (auto i = m_livingEnts.begin(); i != m_livingEnts.end(); ++i)
	{
		if ((*i)->inUse())
		{
			(*i)->update(timer);
		}
	}
}

void Layer_World::render(SpriteBatch& spriteBatch)
{
	Light light;
	light.PosOrDir = MyVec4(0, -1, -1, 0);

	// Mesh objects
	m_mesh_terrain.render(m_camera_main);
	m_shop.render(m_camera_main, light);
	
	// Game objects
	m_towerPool.render(m_camera_main, light);
	m_pawnPool.render(m_camera_main, light);
	m_iFVPool.render(m_camera_main, light);
	m_heroPool.render(m_camera_main, light);
	m_projectilePool.render(m_camera_main);

	for (auto i = m_livingEnts.begin(); i != m_livingEnts.end(); ++i)
	{
		if ((*i)->inUse())
		{
			(*i)->render(spriteBatch, m_camera_main, light);
		}
	}
}

Hero* Layer_World::getPlayer()
{
	return m_heroPool.getPlayer();
}