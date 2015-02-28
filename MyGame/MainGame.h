
#pragma once

#include <FrmApplication.h>

#include <MyPerspectiveCamera.h>
#include <MySpriteBatch.h>
#include <MyFileMesh1.h>

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

	// Core objects
	Timer m_timer;
	SpriteBatch m_spriteBatch;
	PerspectiveCamera m_camera_main;

	// Shader resources
	Shader m_shader_sprite;
	Shader m_shader_terrain;
	Shader m_shader_mesh;

	// Texture resources
	Texture m_texture_grass;
	Texture m_texture_snowman;

	Texture** m_textures_scorpion;

	// Mesh resources
	Adreno::Model* m_meshData_scorpion;

	// Mesh objects
	BasicMesh m_mesh_terrain;

	FileMesh1 m_mesh_scorpion;
};