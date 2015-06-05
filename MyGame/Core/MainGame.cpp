
#include "MainGame.h"
#include "Screen_Menu_Main.h"
#include "Screen_Menu_GameMode.h"
#include "Screen_Play.h"
#include "Screen_GameOver.h"
#include "Screen_Shop.h"

#pragma region Global implementations

//========================================================================================================
//
// Global implementations
//
//========================================================================================================

#ifdef __ANDROID__
const char* g_nativeActivityName = "com/game3d/mygame_android/MyGameNativeActivity";
#endif

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
	// Begin init

	// Shader resources
	m_shader_sprite.init(
		resolveAssetsPath("Shaders/sprite.vs"),
		resolveAssetsPath("Shaders/sprite.fs"),
		Pos2TexVertex::ShaderAttribsDesc,
		Pos2TexVertex::NumShaderAttribsDesc);

	// Core objects
	m_camera2D.init();

	// Global utility objects
	m_userInput.init(m_Input);
	m_spriteBatch.init(m_shader_sprite, m_camera2D);
	//m_gameServer.init();

	// Core objects
	m_screenManager.addScreen("MainMenuScreen", new MainMenuScreen(&m_screenManager));
	m_screenManager.addScreen("GameModeMenuScreen", new GameModeMenuScreen(&m_screenManager));
	m_screenManager.addScreen("PlayScreen", new PlayScreen(&m_screenManager));
	m_screenManager.addScreen("GameOverScreen", new GameOverScreen(&m_screenManager));
	m_screenManager.addScreen("ShopScreen", new ShopScreen(&m_screenManager));
	m_screenManager.activeScreen("MainMenuScreen");

	// End init
	m_initialized = true;

	return Resize();
}

BOOL MainGame::Resize()
{
	if (!m_initialized) return TRUE;

	// Begin resize

	// Global utility objects
	m_userInput.resize(m_nWidth, m_nHeight);
	m_spriteBatch.resize(m_nWidth, m_nHeight);

	// Core objects
	m_screenManager.resize(m_nWidth, m_nHeight);
	m_camera2D.resize(m_nWidth, m_nHeight);

	// End resize
	glViewport( 0, 0, m_nWidth, m_nHeight );

	return TRUE;
}

VOID MainGame::Destroy()
{
	if (!m_initialized) return;

	// Begin destroy

	// End destroy
}

VOID MainGame::Update()
{
	if (!m_initialized) return;

	// Begin update

	// Global utility objects
	m_timer.update();
	m_userInput.update();
	m_spriteBatch.update();

	// Core objects
	{
		GLOBAL_UTIL_OBJS objs =
		{
			&m_timer,
			&m_userInput,
			&m_spriteBatch,
			nullptr,// &m_gameServer,
		};
		m_screenManager.update(&objs);
	}
	m_camera2D.update(m_timer, m_userInput);
	
	// End update
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

	// Begin render

	// Core objects
	{
		GLOBAL_UTIL_OBJS objs =
		{
			&m_timer,
			&m_userInput,
			&m_spriteBatch,
			nullptr,// &m_gameServer,
		};
		m_screenManager.render(&objs);
	}
	
	// End render
}