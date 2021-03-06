
#pragma once

// Always including Utils.h on top
#include "Utils.h"

#include <FrmApplication.h>
#include <MyScreenManager.h>

class MainGame : public CFrmApplication
{
public:
	MainGame(const CHAR* strName);

	virtual BOOL Initialize();
	virtual BOOL Resize();
	virtual VOID Destroy();
	virtual VOID Update();
	virtual VOID Render();

private:
	bool m_initialized;
	bool m_updated;

	// Window
	const CHAR* g_strWindowTitle;
	UINT32 g_nWindowWidth;
	UINT32 g_nWindowHeight;
	FLOAT  g_fAspectRatio;

	// Global utility objects
	Timer m_timer;
	UserInput m_userInput;
	SpriteBatch m_spriteBatch;
	//GameServer m_gameServer;

	// Core objects
	ScreenManager m_screenManager;
	Camera2D m_camera2D;

	// Shader resources
	Shader m_shader_sprite;
};