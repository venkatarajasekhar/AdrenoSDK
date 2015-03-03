
#pragma once

#include <MyScreen.h>

#include <MyPerspectiveCamera.h>
#include <MyBasicMesh.h>
#include <MyFileMesh1.h>

class PlayScreen : public Screen
{
public:
	PlayScreen(ScreenManager* screenManager);
	~PlayScreen();

	void init();
	void resize(int width, int height);
	void update(void* utilObjs);
	void render(void* utilObjs);

private:

	// Core objects
	PerspectiveCamera m_camera_main;

	// Shader resources
	Shader m_shader_terrain;
	Shader m_shader_mesh;

	// Texture resources
	Texture m_texture_grass;
	Texture** m_textures_scorpion;

	// Mesh resources
	Adreno::Model* m_meshData_scorpion;

	// Mesh objects
	BasicMesh m_mesh_terrain;
	FileMesh1 m_mesh_scorpion;
};